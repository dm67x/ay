#include "context.hpp"
#include <fstream>
#include <streambuf>
#include <stb_image.h>

Context::Context(const Window& window)
    : window(window),
    shaders(),
    textures(),
    renderbuffers(),
    framebuffers(),
    currentShader(0)
{
    if (!gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress)) {
        spdlog::critical("cannot initialize glad");
        std::exit(EXIT_FAILURE);
    }

    glCheckError(glEnable(GL_DEPTH_TEST));

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window.window, true);
    ImGui_ImplOpenGL3_Init("#version 300 es");

    shaderFromMemory("default", 
        // VERTEX
        "#version 320 es\n"

        "layout(location = 0) in vec3 position;"
        "layout(location = 1) in vec3 normal;"
        "layout(location = 2) in vec2 uv;"
        "layout(location = 3) in vec3 color;"

        "out VS_OUT{"
            "vec3 position;"
            "vec3 normal;"
            "vec2 uv;"
            "vec3 color;"
        "} vs_out;"

        "uniform mat4 projectionMatrix;"
        "uniform mat4 viewMatrix;"
        "uniform mat4 modelMatrix;"
        "uniform mat4 normalMatrix;"

        "void main() {"
            "vec4 worldPos = modelMatrix * vec4(position, 1.0);"
            "gl_Position = projectionMatrix * viewMatrix * worldPos;"
            "vs_out.position = worldPos.xyz / worldPos.w;"
            "vs_out.normal = (normalMatrix * vec4(normal, 1.0)).xyz;"
            "vs_out.uv = uv;"
            "vs_out.color = color;"
        "}", 
        // FRAGMENT
        "#version 320 es\n"
        "precision mediump float;"

        "out vec4 fragOut;"

        "in VS_OUT{"
            "vec3 position;"
            "vec3 normal;"
            "vec2 uv;"
            "vec3 color;"
        "} fs_in;"

        "uniform bool isAxis;"
        "uniform vec3 cameraPosition;"
        "uniform sampler2D albedo;"

        "struct Light {"
            "vec3 position;"
            "vec4 color;"
            "float intensity;"
        "};"

        "uniform int pointLightsCount;"
        "uniform int directionalLightsCount;"
        "uniform Light pointLights[16];"
        "uniform Light directionalLights[8];"

        "struct Material {"
            "vec4 baseColor;"
            "float metallicFactor;"
            "float roughnessFactor;"
            "vec4 emissiveFactor;"
        "};"

        "uniform Material material;"

        "const vec3 ambientColor = vec3(0.1, 0.1, 0.1);"
        "const vec3 diffuseColor = vec3(0.5, 0.5, 0.5);"
        "const vec3 specColor = vec3(1.0, 1.0, 1.0);"
        "const float shininess = 8.0;"

        "vec3 computeLight(Light light, bool isPoint) {"
            "vec3 N = normalize(fs_in.normal);"
            "vec3 L = light.position - fs_in.position;"
            "float distance = length(L);"
            "distance = distance * distance;"
            "L = normalize(L);"
            "float specular = 0.0;"
            "float lambertian = max(dot(L, N), 0.0);"

            "if (lambertian > 0.0) {"
            "    vec3 V = normalize(cameraPosition - fs_in.position);"
            "    vec3 H = (L + V) / length(L + V);"
            "    float angle = max(dot(H, N), 0.0);"
            "    specular = pow(angle, shininess);"
            "}"

            "float intensity = light.intensity;"
            "if (isPoint) {"
            "    intensity *= 1.0 / distance;"
            "}"

            "return ambientColor +"
            "    material.baseColor.rgb * lambertian * light.color.xyz * intensity +"
            "    specColor * specular * light.color.xyz * intensity;"
        "}"

        "void main() {"
            "vec3 color = vec3(0, 0, 0);"
            // directional lights
            "for (int i = 0; i < directionalLightsCount; i++) {"
                "color += computeLight(directionalLights[i], false);"
            "}"

            // points lights
            "for (int i = 0; i < pointLightsCount; i++) {"
            "    color += computeLight(pointLights[i], true);"
            "}"

            "if (isAxis) {"
                "fragOut = vec4(fs_in.color, 1.0);"
            "}"
            "else {"
                "fragOut = vec4(color, 1.0);"
            "}"
        "}");

    shaderUse("default");
}

Context::~Context() {
    for (auto it = shaders.begin(); it != shaders.end(); it++) {
        glCheckError(glDeleteProgram(it->second));
    }

    for (auto it = textures.begin(); it != textures.end(); it++) {
        glCheckError(glDeleteTextures(1, &it->second));
    }

    for (auto it = renderbuffers.begin(); it != renderbuffers.end(); it++) {
        glCheckError(glDeleteRenderbuffers(1, &it->second));
    }

    for (auto it = framebuffers.begin(); it != framebuffers.end(); it++) {
        glCheckError(glDeleteFramebuffers(1, &it->second));
    }
}

const std::string getProgramLog(GLuint pid) {
    GLint length;
    glCheckError(glGetProgramiv(pid, GL_INFO_LOG_LENGTH, &length));
    std::vector<GLchar> log(length);
    glCheckError(glGetProgramInfoLog(pid, length, nullptr, log.data()));
    return std::string(log.begin(), log.end());
}

const std::string getShaderLog(GLuint sid) {
    GLint length;
    glCheckError(glGetShaderiv(sid, GL_INFO_LOG_LENGTH, &length));
    std::vector<GLchar> log(length);
    glCheckError(glGetShaderInfoLog(sid, length, nullptr, log.data()));
    return std::string(log.begin(), log.end());
}

void Context::shaderFromMemory(const std::string& name, const std::string& vertex, const std::string& fragment) {
    GLuint id, vid, fid;
    glCheckError(id = glCreateProgram());
    glCheckError(vid = glCreateShader(GL_VERTEX_SHADER));
    glCheckError(fid = glCreateShader(GL_FRAGMENT_SHADER));
    std::string log = "";

    const GLchar* vsrc = (const GLchar*)vertex.c_str();
    glCheckError(glShaderSource(vid, 1, &vsrc, nullptr));

    const GLchar* fsrc = (const GLchar*)fragment.c_str();
    glCheckError(glShaderSource(fid, 1, &fsrc, nullptr));

    glCheckError(glCompileShader(vid));
    log = getShaderLog(vid);
    if (log.size() > 0) {
        spdlog::error("Vertex shader: {}", log);
        return;
    }

    glCheckError(glCompileShader(fid));
    log = getShaderLog(fid);
    if (log.size() > 0) {
        spdlog::error("Fragment shader: {}", log);
        return;
    }

    glCheckError(glAttachShader(id, vid));
    glCheckError(glAttachShader(id, fid));
    glCheckError(glLinkProgram(id));

    log = getProgramLog(id);
    if (log.size() > 0) {
        spdlog::error("Program: {}", log);
        return;
    }

    glCheckError(glDetachShader(id, vid));
    glCheckError(glDetachShader(id, fid));
    glCheckError(glDeleteShader(vid));
    glCheckError(glDeleteShader(fid));

    shaders.insert(std::make_pair(name, id));
}

void Context::shaderFromFile(const std::string& name, const std::string& vertex, const std::string& fragment) {
    std::ifstream vfile(vertex, std::ifstream::binary);
    if (!vfile.is_open()) {
        spdlog::error("cannot open file: {}", vertex);
        return;
    }
    std::string vsrc((std::istreambuf_iterator<char>(vfile)), std::istreambuf_iterator<char>());

    std::ifstream ffile(fragment, std::ifstream::binary);
    if (!ffile.is_open()) {
        spdlog::error("cannot open file: {}", fragment);
        return;
    }
    std::string fsrc((std::istreambuf_iterator<char>(ffile)), std::istreambuf_iterator<char>());
    shaderFromMemory(name, vsrc, fsrc);
}

void Context::texture2DNew(const std::string& name, const std::string& filename, Texture2DParameters& params) {
    stbi_set_flip_vertically_on_load(true);

    int channels;
    unsigned char* data = stbi_load(filename.c_str(), &params.width, &params.height, &channels, 0);
    if (!data) {
        spdlog::error("cannot load data from {}", filename);
        return;
    }

    params.internalFormat = channels == 4 ? GL_RGBA : GL_RGB;
    params.dataFormat = channels == 4 ? GL_RGBA : GL_RGB;

    GLuint id;
    glCheckError(glGenTextures(1, &id));
    glCheckError(glBindTexture(GL_TEXTURE_2D, id));
    glCheckError(glTexImage2D(GL_TEXTURE_2D, params.lod,
        params.internalFormat, params.width, params.height, 0,
        params.dataFormat, params.dataType, data));
    glCheckError(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, params.min));
    glCheckError(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, params.mag));
    glCheckError(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, params.wrapT));
    glCheckError(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, params.wrapS));
    if (params.mipmap) {
        glCheckError(glGenerateMipmap(GL_TEXTURE_2D));
    }
    glCheckError(glBindTexture(GL_TEXTURE_2D, 0));
    textures.insert(std::make_pair(name, id));
    stbi_image_free(data);
}

void Context::framebufferNew(const std::string& name, const std::array<GLuint, 32>& colorAttachments, GLuint depthStencilAttachment) {
    GLuint id;
    glCheckError(glGenFramebuffers(1, &id));
    glCheckError(glBindFramebuffer(GL_FRAMEBUFFER, id));
    for (int i = 0; i < 32; i++) {
        glCheckError(glFramebufferTexture2D(GL_FRAMEBUFFER, (GLenum)(GL_COLOR_ATTACHMENT0 + i), GL_TEXTURE_2D, colorAttachments[i], 0));
    }

    glCheckError(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthStencilAttachment));

    GLenum status;
    glCheckError(status = glCheckFramebufferStatus(GL_FRAMEBUFFER));
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        spdlog::error("The framebuffer is not complete");
        return;
    }

    glCheckError(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    framebuffers.insert(std::make_pair(name, id));
}
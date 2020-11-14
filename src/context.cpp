#include "context.hpp"
#include "math.hpp"
#include <fstream>
#include <streambuf>
#include <stb_image.h>

Context::~Context() {
    for (auto it = shaders.begin(); it != shaders.end(); it++) {
        delete it->second;
    }

    for (auto it = textures.begin(); it != textures.end(); it++) {
        delete it->second;
    }

    for (auto it = renderbuffers.begin(); it != renderbuffers.end(); it++) {
        delete it->second;
    }

    for (auto it = framebuffers.begin(); it != framebuffers.end(); it++) {
        delete it->second;
    }
}

void Context::clear(float r, float g, float b, float a) const {
    platform->clear(r, g, b, a);
}

void Context::viewport(int x, int y, int w, int h) const {
    platform->viewport(x, y, w, h);
}

ShaderProgram* Context::shaderFromMemory(const std::string& name, const std::string& vertex, const std::string& fragment) {
    auto program = platform->newProgram(vertex, fragment);
    shaders.insert(std::make_pair(name, program));
    return program;
}

ShaderProgram* Context::shaderFromFile(const std::string& name, const std::string& vertex, const std::string& fragment) {
    std::ifstream vfile(vertex, std::ifstream::binary);
    if (!vfile.is_open()) {
        spdlog::error("cannot open file: {}", vertex);
        return nullptr;
    }
    std::string vsrc((std::istreambuf_iterator<char>(vfile)), std::istreambuf_iterator<char>());

    std::ifstream ffile(fragment, std::ifstream::binary);
    if (!ffile.is_open()) {
        spdlog::error("cannot open file: {}", fragment);
        return nullptr;
    }
    std::string fsrc((std::istreambuf_iterator<char>(ffile)), std::istreambuf_iterator<char>());
    return shaderFromMemory(name, vsrc, fsrc);
}

void Context::shaderDestroy(const std::string& name) {
    auto it = shaders.find(name);
    if (it != shaders.end()) {
        delete it->second;
        shaders.erase(it);
    }
}

void Context::shaderUse(const std::string& name) {
    auto shader = shaders.find(name);
    if (shader != shaders.end()) {
        currentShader = shader->second;
    }
    currentShader->use();
}

void Context::shaderUniform(const std::string& name, float value) const {
    currentShader->uniform1f(name, value);
}

void Context::shaderUniform(const std::string& name, int value) const {
    currentShader->uniform1i(name, value);
}

void Context::shaderUniform(const std::string& name, const Vec3& value) const {
    currentShader->uniform3f(name, new float[3] { value.x, value.y, value.z });
}

void Context::shaderUniform(const std::string& name, const Mat4& value) const {
    float values[16];
    std::memcpy(values, value.r1, sizeof(value.r1));
    std::memcpy(values + 4, value.r2, sizeof(value.r2));
    std::memcpy(values + 8, value.r3, sizeof(value.r3));
    std::memcpy(values + 12, value.r4, sizeof(value.r4));
    currentShader->uniformMatrix(name, &values[0], true);
}

VertexArrayObject* Context::vertexArrayObjectNew() const {
    return platform->newVertexArrayObject();
}

Buffer* Context::bufferNew() const {
    return platform->newBuffer();
}

Texture2D* Context::texture2DNew(const std::string& name, int width, int height) {
    auto tex = platform->newTexture2D(width, height, nullptr, TextureParameters());
    textures.insert(std::make_pair(name, tex));
    return tex;
}

Texture2D* Context::texture2DNew(const std::string& name, const std::string& filename) {
    stbi_set_flip_vertically_on_load(true);

    int width, height, channels;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &channels, 0);
    if (!data) {
        spdlog::error("cannot load data from {}", filename);
        return nullptr;
    }

    TextureParameters params;
    params.internalFormat = channels == 4 ? TextureFormat::RGBA : TextureFormat::RGB;
    params.dataFormat = channels == 4 ? TextureFormat::RGBA : TextureFormat::RGB;
    params.mipMap = true;
    auto tex = platform->newTexture2D(width, height, data, params);
    stbi_image_free(data);
    textures.insert(std::make_pair(name, tex));
    return tex;
}

void Context::texture2DDestroy(const std::string& name) {
    auto it = textures.find(name);
    if (it != textures.end()) {
        delete it->second;
        textures.erase(it);
    }
}

Texture2D* Context::texture2DGet(const std::string& name) const {
    auto it = textures.find(name);
    if (it != textures.end()) {
        return it->second;
    }
    return nullptr;
}

Renderbuffer* Context::renderbufferNew(const std::string& name, int width, int height) {
    auto rb = platform->newRenderbuffer(width, height);
    renderbuffers.insert(std::make_pair(name, rb));
    return rb;
}

void Context::renderbufferDestroy(const std::string& name) {
    auto it = renderbuffers.find(name);
    if (it != renderbuffers.end()) {
        delete it->second;
        renderbuffers.erase(it);
    }
}

Renderbuffer* Context::renderbufferGet(const std::string& name) const {
    auto it = renderbuffers.find(name);
    if (it != renderbuffers.end()) {
        return it->second;
    }
    return nullptr;
}

Framebuffer* Context::framebufferNew(const std::string& name, const std::array<Texture2D*, 32>& colorAttachments, Renderbuffer* depthStencilAttachment) {
    FramebufferParameters params;
    for (size_t i = 0; i < colorAttachments.size(); i++) {
        params.colorAttachments[i] = colorAttachments[i];
    }
    params.depthStencilAttachment = depthStencilAttachment;
    auto fb = platform->newFramebuffer(params);
    framebuffers.insert(std::make_pair(name, fb));
    return fb;
}

void Context::framebufferDestroy(const std::string& name) {
    auto it = framebuffers.find(name);
    if (it != framebuffers.end()) {
        delete it->second;
        framebuffers.erase(it);
    }
}

Framebuffer* Context::framebufferGet(const std::string& name) const {
    auto it = framebuffers.find(name);
    if (it != framebuffers.end()) {
        return it->second;
    }
    return nullptr;
}
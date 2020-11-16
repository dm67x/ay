#pragma once

#include "math.hpp"
#include <spdlog/spdlog.h>
#include <glad.h>
#include <GLFW/glfw3.h>
#include <map>
#include <array>
#include <sstream>

#ifdef NDEBUG
#define glCheckError(expr) expr
#else
#define glCheckError(expr) {\
    expr;\
    switch (glGetError()) {\
        case GL_INVALID_ENUM: \
            spdlog::error("OpenGL error (INVALID_ENUM) in {} at line: {}", __FILE__, __LINE__); \
            break; \
        case GL_INVALID_VALUE: \
            spdlog::error("OpenGL error (INVALID_VALUE) in {} at line: {}", __FILE__, __LINE__); \
            break; \
        case GL_INVALID_OPERATION: \
            spdlog::error("OpenGL error (INVALID_OPERATION) in {} at line: {}", __FILE__, __LINE__); \
            break; \
        case GL_STACK_OVERFLOW: \
            spdlog::error("OpenGL error (STACK_OVERFLOW) in {} at line: {}", __FILE__, __LINE__); \
            break; \
        case GL_STACK_UNDERFLOW: \
            spdlog::error("OpenGL error (STACK_UNDERFLOW) in {} at line: {}", __FILE__, __LINE__); \
            break; \
        case GL_OUT_OF_MEMORY: \
            spdlog::error("OpenGL error (OUT_OF_MEMORY) in {} at line: {}", __FILE__, __LINE__); \
            break; \
        case GL_INVALID_FRAMEBUFFER_OPERATION: \
            spdlog::error("OpenGL error (INVALID_FRAMEBUFFER_OPERATION) in {} at line: {}", __FILE__, __LINE__); \
            break; \
    }}
#endif

using Shader = GLuint;
using Texture2D = GLuint;
using Renderbuffer = GLuint;
using Framebuffer = GLuint;
using VAO = GLuint;
using Buffer = GLuint;
using Attribute = GLuint;

enum class DrawMethod {
    ARRAY,
    ELEMENT,
    INSTANCE
};

struct DrawParameters {
    GLenum mode;
    union {
        GLint first;
        GLenum elementType;
    };
    GLsizei count;
    GLvoid* offset;

    DrawParameters(GLenum mode, GLint first, GLsizei count)
        : mode(mode),
        first(first),
        count(count),
        offset(nullptr)
    {
    }

    DrawParameters(GLenum mode, GLenum elementType, GLsizei count, GLvoid* offset)
        : mode(mode),
        elementType(elementType),
        count(count),
        offset(offset)
    {
    }
};

enum class BufferUsage {
    ARRAY = GL_ARRAY_BUFFER,
    ELEMENT = GL_ELEMENT_ARRAY_BUFFER
};

enum class BufferTarget {
    STATIC_DRAW = GL_STATIC_DRAW,
    STATIC_COPY = GL_STATIC_COPY,
    STATIC_READ = GL_STATIC_READ,
    DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
    DYNAMIC_COPY = GL_DYNAMIC_COPY,
    DYNAMIC_READ = GL_DYNAMIC_READ,
};

struct Texture2DParameters {
    GLint mag = GL_LINEAR;
    GLint min = GL_LINEAR;
    GLint wrapS = GL_REPEAT;
    GLint wrapT = GL_REPEAT;
    GLenum internalFormat = GL_RGBA;
    GLenum dataFormat = GL_RGBA;
    GLenum dataType = GL_UNSIGNED_BYTE;
    int lod = 0;
    bool mipmap = false;
    int width = 0;
    int height = 0;
};

class Context {
    std::map<std::string, Shader> shaders;
    std::map<std::string, Texture2D> textures;
    std::map<std::string, Renderbuffer> renderbuffers;
    std::map<std::string, Framebuffer> framebuffers;
    Shader currentShader;

public:
    ///
    /// @brief Constructor of context
    ///
    Context() 
        : shaders(), 
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
    }

    ///
    /// @brief Destructor of context
    ///
    ~Context();

    ///
    /// @brief Get OpenGL Version
    /// @return Version
    ///
    inline const std::string getVersion() {
        std::stringstream version;
        version << glGetString(GL_VERSION);
        return version.str();
    }

    ///
    /// @brief Get Vendor
    /// @return Vendor
    ///
    inline const std::string getVendor() {
        std::stringstream vendor;
        vendor << glGetString(GL_VENDOR);
        return vendor.str();
    }

    ///
    /// @brief Clear screen
    /// @param r Red
    /// @param g Green
    /// @param b Blue
    /// @param a Alpha
    /// 
    inline void clear(float r = 0.f, float g = 0.f, float b = 0.f, float a = 1.f) const {
        glCheckError(glClearColor(r, g, b, a));
        glCheckError(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    }

    ///
    /// @brief Set viewport
    /// @param x Left
    /// @param y Top
    /// @param w Width
    /// @param h Height
    /// 
    inline void viewport(int x, int y, int w, int h) const {
        glCheckError(glViewport((GLint)x, (GLint)y, (GLsizei)w, (GLsizei)h));
    }

    ///
    /// @brief Create shader from memory
    /// @param name shader name
    /// @param vertex vertex shader
    /// @param fragment fragment shader
    /// @return ShaderProgram instance
    void shaderFromMemory(const std::string& name, const std::string& vertex, const std::string& fragment);

    ///
    /// @brief Create shader from file
    /// @param name shader name
    /// @param vertex vertex shader
    /// @param fragment fragment shader
    /// @return ShaderProgram instance
    void shaderFromFile(const std::string& name, const std::string& vertex, const std::string& fragment);

    /// 
    /// @brief Destroy the shader
    /// @param name Shader name
    /// 
    inline void shaderDispose(const std::string& name) {
        auto it = shaders.find(name);
        if (it != shaders.end()) {
            glCheckError(glDeleteProgram(it->second));
            shaders.erase(it);
        }
    }

    ///
    /// @brief Use shader
    /// @param name Shader name
    ///
    inline void shaderUse(const std::string& name) {
        auto shader = shaders.find(name);
        if (shader != shaders.end()) {
            currentShader = shader->second;
        }
        glCheckError(glUseProgram(currentShader));
    }

    ///
    /// @brief Shader uniform1f
    /// @param name uniform name
    /// @param value uniform value
    ///
    inline void shaderUniform(const std::string& name, float value) const {
        GLint loc;
        glCheckError(loc = glGetUniformLocation(currentShader, name.c_str()));
        glCheckError(glUniform1f(loc, value));
    }

    ///
    /// @brief Shader uniform1i
    /// @param name uniform name
    /// @param value uniform value
    ///
    inline void shaderUniform(const std::string& name, int value) const {
        GLint loc;
        glCheckError(loc = glGetUniformLocation(currentShader, name.c_str()));
        glCheckError(glUniform1i(loc, value));
    }

    ///
    /// @brief Shader uniform3fv
    /// @param name uniform name
    /// @param value uniform value
    ///
    inline void shaderUniform(const std::string& name, const Vec3& value) const {
        GLint loc;
        glCheckError(loc = glGetUniformLocation(currentShader, name.c_str()));
        glCheckError(glUniform3fv(loc, 1, new float[3]{ value.x, value.y, value.z }));
    }

    ///
    /// @brief Shader uniform4fv
    /// @param name uniform name
    /// @param value uniform value
    ///
    inline void shaderUniform(const std::string& name, const Vec4& value) const {
        GLint loc;
        glCheckError(loc = glGetUniformLocation(currentShader, name.c_str()));
        glCheckError(glUniform4fv(loc, 1, new float[4]{ value.x, value.y, value.z, value.w }));
    }

    ///
    /// @brief Shader uniformMatrix4fv
    /// @param name uniform name
    /// @param value uniform value
    ///
    inline void shaderUniform(const std::string& name, const Mat4& value) const {
        GLint loc;
        glCheckError(loc = glGetUniformLocation(currentShader, name.c_str()));

        float values[16];
        std::memcpy(values, value.r1, sizeof(value.r1));
        std::memcpy(values + 4, value.r2, sizeof(value.r2));
        std::memcpy(values + 8, value.r3, sizeof(value.r3));
        std::memcpy(values + 12, value.r4, sizeof(value.r4));

        glCheckError(glUniformMatrix4fv(loc, 1, GL_TRUE, values));
    }

    ///
    /// @brief Create a new vao
    /// @return VAO id
    ///
    inline VAO vaoNew() const {
        VAO id;
        glCheckError(glGenVertexArrays(1, &id));
        return id;
    }

    ///
    /// @brief Destroy VAO
    /// @param id VAO id
    ///
    inline void vaoDispose(VAO id) const {
        glCheckError(glDeleteVertexArrays(1, &id));
    }

    ///
    /// @brief Use VAO
    /// @param id VAO id
    ///
    inline void vaoUse(VAO id) const {
        glCheckError(glBindVertexArray(id));
    }

    ///
    /// @brief Draw call
    /// @param params Draw parameters
    ///
    template<DrawMethod T>
    inline void draw(DrawParameters params) const {}

    ///
    /// @brief Draw call (array)
    /// @param params Draw parameters
    ///
    template<>
    inline void draw<DrawMethod::ARRAY>(DrawParameters params) const {
        glCheckError(glDrawArrays(params.mode, params.first, params.count));
    }

    ///
    /// @brief Draw call (element)
    /// @param params Draw parameters
    ///
    template<>
    inline void draw<DrawMethod::ELEMENT>(DrawParameters params) const {
        glCheckError(glDrawElements(params.mode, params.count, params.elementType, params.offset));
    }

    ///
    /// @brief Create a new buffer
    /// @return Buffer id
    ///
    inline Buffer bufferNew() const {
        Buffer id;
        glCheckError(glGenBuffers(1, &id));
        return id;
    }

    ///
    /// @brief Destroy the buffer
    /// @param id Buffer id
    ///
    inline void bufferDispose(Buffer id) const {
        glCheckError(glDeleteBuffers(1, &id));
    }

    ///
    /// @brief Buffer use
    /// @param id Buffer id
    ///
    template<BufferUsage T>
    inline void bufferUse(Buffer id) const {
        glCheckError(glBindBuffer((GLenum)T, id));
    }

    ///
    /// @brief Buffer set data
    /// @param size Size
    /// @param data Data
    ///
    template<BufferUsage T, BufferTarget T2>
    inline void bufferData(GLsizeiptr size, GLvoid* data) const {
        glCheckError(glBufferData((GLenum)T, size, data, (GLenum)T2));
    }

    ///
    /// @brief Buffer set data
    /// @param offset Offset
    /// @param size Size
    /// @param data Data
    ///
    template<BufferUsage T>
    inline void bufferSubData(GLintptr offset, GLsizeiptr size, GLvoid* data) const {
        glCheckError(glBufferSubData((GLenum)T, offset, size, data));
    }

    ///
    /// @brief Buffer attribute array
    /// @param index Attribute index
    /// @param type Attribute type
    /// @param size Specifies the number of components per generic vertex attribute
    /// @param stride Specifies the byte offset between consecutive generic vertex attributes
    /// @param offset Specifies a offset of the first component of the first generic vertex attribute
    ///
    inline void bufferAttribute(Attribute index, GLenum type, GLint size, GLsizei stride, GLvoid* offset) const {
        glCheckError(glEnableVertexAttribArray(index));
        glCheckError(glVertexAttribPointer(index, size, type, GL_FALSE, stride, offset));
    }

    /// 
    /// @brief Create a new empty texture
    /// @param name Texture name
    /// @param params TextureParameters
    /// 
    inline void texture2DNew(const std::string& name, Texture2DParameters params) {
        Texture2D id;
        glCheckError(glGenTextures(1, &id));
        glCheckError(glBindTexture(GL_TEXTURE_2D, id));
        glCheckError(glTexImage2D(GL_TEXTURE_2D, params.lod,
            params.internalFormat, params.width, params.height, 0, 
            params.dataFormat, params.dataType, nullptr));
        glCheckError(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, params.min));
        glCheckError(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, params.mag));
        glCheckError(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, params.wrapT));
        glCheckError(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, params.wrapS));
        if (params.mipmap) {
            glCheckError(glGenerateMipmap(GL_TEXTURE_2D));
        }
        glCheckError(glBindTexture(GL_TEXTURE_2D, 0));
        textures.insert(std::make_pair(name, id));
    }

    /// 
    /// @brief Create a new texture from file
    /// @param name Texture name
    /// @param filename Filename
    /// @param params TextureParameters ref
    /// 
    void texture2DNew(const std::string& name, const std::string& filename, Texture2DParameters& params);

    ///
    /// @brief Destroy the texture
    /// @param name Texture name
    /// 
    inline void texture2DDispose(const std::string& name) {
        auto it = textures.find(name);
        if (it != textures.end()) {
            glCheckError(glDeleteTextures(1, &it->second));
            textures.erase(it);
        }
    }

    ///
    /// @brief Use texture
    /// @param name Texture name
    /// 
    template<GLuint S = 0>
    inline void texture2DUse(const std::string& name) const {
        Texture2D id = texture2DGet(name);
        glCheckError(glActiveTexture(GL_TEXTURE0 + S));
        glCheckError(glBindTexture(GL_TEXTURE_2D, id));
    }

    /// 
    /// @brief Get the texture by name
    /// @param name Texture name
    /// @return Texture id
    /// 
    inline Texture2D texture2DGet(const std::string& name) const {
        auto it = textures.find(name);
        if (it != textures.end()) {
            return it->second;
        }
        return 0;
    }

    /// 
    /// @brief Create a new renderbuffer
    /// @param name Renderbuffer name
    /// @param width Width
    /// @param height Height
    /// 
    inline void renderbufferNew(const std::string& name, int width, int height) {
        Renderbuffer id;
        glCheckError(glGenRenderbuffers(1, &id));
        glCheckError(glBindRenderbuffer(GL_RENDERBUFFER, id));
        glCheckError(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));
        glCheckError(glBindRenderbuffer(GL_RENDERBUFFER, 0));
        renderbuffers.insert(std::make_pair(name, id));
    }

    /// 
    /// @brief Destroy the renderbuffer
    /// @param name Renderbuffer name
    /// 
    inline void renderbufferDispose(const std::string& name) {
        auto it = renderbuffers.find(name);
        if (it != renderbuffers.end()) {
            glCheckError(glDeleteRenderbuffers(1, &it->second));
            renderbuffers.erase(it);
        }
    }

    /// 
    /// @brief Get the renderbuffer by name
    /// @param name Renderbuffer name
    /// @return Renderbuffer id
    /// 
    inline Renderbuffer renderbufferGet(const std::string& name) const {
        auto it = renderbuffers.find(name);
        if (it != renderbuffers.end()) {
            return it->second;
        }
        return 0;
    }

    /// 
    /// @brief Create a new framebuffer
    /// @param name Framebuffer name
    /// @param colorAttachments Color Attachments (max 32)
    /// @param depthStencilAttachment Depth&Stencil attachment
    /// 
    void framebufferNew(const std::string& name, const std::array<GLuint, 32>& colorAttachments, GLuint depthStencilAttachment = 0);

    /// 
    /// @brief Destroy the framebuffer
    /// @param name Framebuffer name
    /// 
    inline void framebufferDispose(const std::string& name) {
        auto it = framebuffers.find(name);
        if (it != framebuffers.end()) {
            glCheckError(glDeleteFramebuffers(1, &it->second));
            framebuffers.erase(it);
        }
    }

    /// 
    /// @brief Use framebuffer
    /// @param name Framebuffer name
    /// 
    inline void framebufferUse(const std::string& name) const {
        Framebuffer id = framebufferGet(name);
        glCheckError(glBindFramebuffer(GL_FRAMEBUFFER, id));
    }

    /// 
    /// @brief Get the framebuffer
    /// @param name Framebuffer name
    /// @return Framebuffer id
    ///
    inline Framebuffer framebufferGet(const std::string& name) const {
        auto it = framebuffers.find(name);
        if (it != framebuffers.end()) {
            return it->second;
        }
        return 0;
    }
};
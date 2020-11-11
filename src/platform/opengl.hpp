#pragma once

#include <GLFW/glfw3.h>
#include <glad.h>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <cstring>

#ifdef NDEBUG
#define glCheckError(expr) expr
#else
#define glCheckError(expr) {\
    expr;\
    Error err = OpenGL::getError();\
    if (err != Error::NO_ERROR) {\
        std::cerr << "OpenGL error at line: " << __LINE__ << std::endl;\
    } }
#endif

using PlatformId = GLuint;

class OpenGL {
    friend class Context;

public:
    enum class Error {
        NO_ERROR,
        INVALID_ENUM,
        INVALID_VALUE,
        INVALID_OPERATION,
        STACK_OVERFLOW,
        STACK_UNDERFLOW,
        OUT_OF_MEMORY,
        INVALID_FRAMEBUFFER_OPERATION
    };

    enum class TextureWrap {
        REPEAT = GL_REPEAT,
        MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
        CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
        CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER
    };

    enum class TextureFiltering {
        NEAREST = GL_NEAREST,
        LINEAR = GL_LINEAR
    };

    enum class DrawMode {
        TRIANGLES = GL_TRIANGLES,
        TRIANGLE_STRIP_ADJACENCY = GL_TRIANGLE_STRIP_ADJACENCY,
        TRIANGLES_ADJACENCY = GL_TRIANGLES_ADJACENCY,
        POINTS = GL_POINTS,
        LINE_STRIP = GL_LINE_STRIP,
        LINE_LOOP = GL_LINE_LOOP,
        LINES = GL_LINES,
        LINE_STRIP_ADJACENCY = GL_LINE_STRIP_ADJACENCY,
        LINES_ADJACENCY = GL_LINES_ADJACENCY,
        TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
        TRIANGLE_FAN = GL_TRIANGLE_FAN,
        PATCHES = GL_PATCHES
    };

    enum class BufferMode {
        ARRAY = GL_ARRAY_BUFFER,
        ELEMENT_ARRAY = GL_ELEMENT_ARRAY_BUFFER
    };

    enum class BufferTarget {
        STATIC_DRAW = GL_STATIC_DRAW,
        STATIC_COPY = GL_STATIC_COPY,
        STATIC_READ = GL_STATIC_READ,
        DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
        DYNAMIC_COPY = GL_DYNAMIC_COPY,
        DYNAMIC_READ = GL_DYNAMIC_READ,
    };

    enum class AttribType {
        FLOAT = GL_FLOAT,
        UNSIGNED_INT = GL_UNSIGNED_INT,
        BYTE = GL_BYTE,
        UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
        INT = GL_INT
    };

    enum class TextureType {
        UNSIGNED_BYTE = GL_UNSIGNED_BYTE, 
        BYTE = GL_BYTE, 
        UNSIGNED_SHORT = GL_UNSIGNED_SHORT, 
        SHORT = GL_SHORT, 
        UNSIGNED_INT = GL_UNSIGNED_INT, 
        INT = GL_INT, 
        HALF_FLOAT = GL_HALF_FLOAT, 
        FLOAT = GL_FLOAT
    };

    enum class TextureFormat {
        RED = GL_RED, 
        RG = GL_RG, 
        RGB = GL_RGB,  
        RGBA = GL_RGBA, 
        RED_INTEGER = GL_RED_INTEGER, 
        RG_INTEGER = GL_RG_INTEGER, 
        RGB_INTEGER = GL_RGB_INTEGER, 
        RGBA_INTEGER = GL_RGBA_INTEGER, 
        STENCIL_INDEX = GL_STENCIL_INDEX, 
        DEPTH_COMPONENT = GL_DEPTH_COMPONENT, 
        DEPTH_STENCIL = GL_DEPTH_STENCIL
    };

    ///
    /// @brief Get OpenGL Version
    /// @return Version
    ///
    inline static const std::string getVersion() {
        std::stringstream version;
        version << glGetString(GL_VERSION);
        return version.str();
    }

    ///
    /// @brief Get OpenGL Vendor
    /// @return Vendor
    ///
    inline static const std::string getVendor() {
        std::stringstream version;
        version << glGetString(GL_VENDOR);
        return version.str();
    }

    ///
    /// @brief Load OpenGL Context
    /// @return True if context loaded false otherwise
    ///
    inline static bool loadContext() {
        return gladLoadGLES2Loader((GLADloadproc)glfwGetProcAddress);
    }

private:
    ///
    /// @brief Get OpenGL error
    /// @return Error code
    ///
    inline static Error getError() {
        switch (glGetError()) {
            case GL_INVALID_ENUM: return Error::INVALID_ENUM;
            case GL_INVALID_VALUE: return Error::INVALID_VALUE;
            case GL_INVALID_OPERATION: return Error::INVALID_OPERATION;
            case GL_STACK_OVERFLOW: return Error::STACK_OVERFLOW;
            case GL_STACK_UNDERFLOW: return Error::STACK_UNDERFLOW;
            case GL_OUT_OF_MEMORY: return Error::OUT_OF_MEMORY;
            case GL_INVALID_FRAMEBUFFER_OPERATION: return Error::INVALID_FRAMEBUFFER_OPERATION;
            default: return Error::NO_ERROR;
        }
    }

    ///
    /// @brief Clear screen
    /// @param r Red
    /// @param g Green
    /// @param b Blue
    /// @param a Alpha
    /// 
    inline static void clear(float r, float g, float b, float a) {
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
    inline static void viewport(int x, int y, int w, int h) {
        glCheckError(glViewport((GLint)x, (GLint)y, (GLsizei)w, (GLsizei)h));
    }

    ///
    /// @brief Create a new shader program
    /// @param vertex vertex source
    /// @param fragment fragment source
    /// @return id Program id
    ///
    static PlatformId shaderNew(const std::string& vertex, const std::string& fragment) {
        PlatformId id, vid, fid;
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
            std::cerr << "Vertex: " << log << std::endl;
            return 0;
        }

        glCheckError(glCompileShader(fid));
        log = getShaderLog(fid);
        if (log.size() > 0) {
            std::cerr << "Fragment: " << log << std::endl;
            return 0;
        }

        glCheckError(glAttachShader(id, vid));
        glCheckError(glAttachShader(id, fid));
        glCheckError(glLinkProgram(id));

        log = getProgramLog(id);
        if (log.size() > 0) {
            std::cerr << "Program: " << log << std::endl;
            return 0;
        }

        glCheckError(glDetachShader(id, vid));
        glCheckError(glDetachShader(id, fid));
        glCheckError(glDeleteShader(vid));
        glCheckError(glDeleteShader(fid));
        return id;
    }

    ///
    /// @brief Destroy shader program
    /// @param id Program id
    ///
    inline static void shaderDestroy(PlatformId id) {
        glCheckError(glDeleteProgram(id));
    }

    ///
    /// @brief Use shader program
    /// @param id program id
    ///
    inline static void shaderUse(PlatformId id) {
        glCheckError(glUseProgram(id));
    }

    ///
    /// @brief Get program log
    /// @param id program id
    /// @return log
    /// 
    inline static const std::string getProgramLog(PlatformId id) {
        GLint length;
        glCheckError(glGetProgramiv(id, GL_INFO_LOG_LENGTH, &length));
        std::vector<GLchar> log(length);
        glCheckError(glGetProgramInfoLog(id, length, nullptr, log.data()));
        return std::string(log.begin(), log.end());
    }

    ///
    /// @brief Uniform1f
    /// @param id Program id
    /// @param name Uniform name
    /// @param value Value
    ///
    inline static PlatformId shaderUniform1f(PlatformId id, const std::string& name, float value) {
        GLint loc;
        glCheckError(loc = glGetUniformLocation(id, name.c_str()));
        glCheckError(glUniform1f(loc, value));
        return id;
    }

    ///
    /// @brief Uniform1i
    /// @param id Program id
    /// @param name Uniform name
    /// @param value Value
    ///
    inline static PlatformId shaderUniform1i(PlatformId id, const std::string& name, int value) {
        GLint loc;
        glCheckError(loc = glGetUniformLocation(id, name.c_str()));
        glCheckError(glUniform1i(loc, value));
        return id;
    }

    ///
    /// @brief Uniform3fv
    /// @param id Program id
    /// @param name Uniform name
    /// @param value Value
    ///
    inline static PlatformId shaderUniform3fv(PlatformId id, const std::string& name, float value[3]) {
        GLint loc;
        glCheckError(loc = glGetUniformLocation(id, name.c_str()));
        glCheckError(glUniform3fv(loc, 1, value));
        return id;
    }

    ///
    /// @brief Uniform4fv
    /// @param id Program id
    /// @param name Uniform name
    /// @param value Value
    ///
    inline static PlatformId shaderUniform4fv(PlatformId id, const std::string& name, float value[4]) {
        GLint loc;
        glCheckError(loc = glGetUniformLocation(id, name.c_str()));
        glCheckError(glUniform4fv(loc, 1, value));
        return id;
    }

    ///
    /// @brief UniformMatrix4fv
    /// @param id Program id
    /// @param name Uniform name
    /// @param value Value
    /// @param transpose Transpose matrix
    ///
    inline static PlatformId shaderUniformMatrix4fv(PlatformId id, const std::string& name, float value[16], bool transpose = false) {
        GLint loc;
        glCheckError(loc = glGetUniformLocation(id, name.c_str()));
        glCheckError(glUniformMatrix4fv(loc, 1, transpose, value));
        return id;
    }

    ///
    /// @brief Get shader log
    /// @param id shader id
    /// @return log
    ///
    inline static const std::string getShaderLog(PlatformId id) {
        GLint length;
        glCheckError(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        std::vector<GLchar> log(length);
        glCheckError(glGetShaderInfoLog(id, length, nullptr, log.data()));
        return std::string(log.begin(), log.end());
    }

    /// 
    /// @brief TextureParameters
    /// 
    struct TextureParameters {
        TextureFiltering mag;
        TextureFiltering min;
        TextureWrap wrapS;
        TextureWrap wrapT;
        TextureFormat internalFormat;
        TextureFormat dataFormat;
        TextureType dataType;
        int levelOfDetail;
        bool mipMap;

        TextureParameters()
            : mag(TextureFiltering::LINEAR),
            min(TextureFiltering::LINEAR),
            wrapS(TextureWrap::REPEAT),
            wrapT(TextureWrap::REPEAT),
            internalFormat(TextureFormat::RGBA),
            dataFormat(TextureFormat::RGBA),
            dataType(TextureType::UNSIGNED_BYTE),
            levelOfDetail(0),
            mipMap(false)
        {
        }
    };

    ///
    /// @brief Create a new texture
    /// @return texture id
    ///
    static PlatformId textureNew(int width, int height, const void* data, TextureParameters params) {
        PlatformId id;
        glCheckError(glGenTextures(1, &id));
        glCheckError(glBindTexture(GL_TEXTURE_2D, id));
        glCheckError(glTexImage2D(GL_TEXTURE_2D, params.levelOfDetail, 
            (GLenum)params.internalFormat, width, height, 0, (GLenum)params.dataFormat, 
            (GLenum)params.dataType, data));
        glCheckError(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)params.min));
        glCheckError(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)params.mag));
        glCheckError(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint)params.wrapT));
        glCheckError(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint)params.wrapS));
        if (params.mipMap) {
            glCheckError(glGenerateMipmap(GL_TEXTURE_2D));
        }
        glCheckError(glBindTexture(GL_TEXTURE_2D, id));
        return id;
    }

    ///
    /// @brief Destroy the texture
    /// @param id texture id
    ///
    inline static void textureDestroy(PlatformId id) {
        glCheckError(glDeleteTextures(1, &id));
    }

    ///
    /// @brief Bind texture
    /// @param id texture id
    ///
    inline static void textureUse(PlatformId id, unsigned char slot = 0) {
        glCheckError(glActiveTexture(GL_TEXTURE0 + slot));
        glCheckError(glBindTexture(GL_TEXTURE_2D, id));
    }

    ///
    /// @brief Create a new renderbuffer
    /// @param width Renderbuffer width
    /// @param height Renderbuffer height
    /// @return Renderbuffer id
    /// 
    static PlatformId renderbufferNew(int width, int height) {
        PlatformId id;
        glCheckError(glGenRenderbuffers(1, &id));
        glCheckError(glBindRenderbuffer(GL_RENDERBUFFER, id));
        glCheckError(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));
        glCheckError(glBindRenderbuffer(GL_RENDERBUFFER, 0));
        return id;
    }

    ///
    /// @brief Destroy the renderbuffer
    /// @param id Renderbuffer id
    /// 
    inline static void renderbufferDestroy(PlatformId id) {
        glCheckError(glDeleteRenderbuffers(1, &id));
    }

    /// 
    /// @brief FramebufferParams
    /// 
    struct FramebufferParameters {
        PlatformId colorAttachments[32];
        PlatformId depthStencilAttachment;

        FramebufferParameters() {
            std::memset(colorAttachments, 0, sizeof(colorAttachments));
            depthStencilAttachment = 0;
        }
    };

    static PlatformId framebufferNew(const FramebufferParameters& params) {
        PlatformId id;
        glCheckError(glGenFramebuffers(1, &id));
        glCheckError(glBindFramebuffer(GL_FRAMEBUFFER, id));
        for (int i = 0; i < 32; i++) {
            if (params.colorAttachments[i] > 0) {
                glCheckError(glFramebufferTexture2D(
                    GL_FRAMEBUFFER, 
                    (GLenum)(GL_COLOR_ATTACHMENT0 + i), 
                    GL_TEXTURE_2D, 
                    params.colorAttachments[i], 0));
            }
        }
        
        if (params.depthStencilAttachment != 0) {
            glCheckError(glFramebufferRenderbuffer(
                GL_FRAMEBUFFER, 
                GL_DEPTH_STENCIL_ATTACHMENT, 
                GL_RENDERBUFFER, 
                params.depthStencilAttachment));
        }

        GLenum status;
        glCheckError(status = glCheckFramebufferStatus(GL_FRAMEBUFFER));
        if (status != GL_FRAMEBUFFER_COMPLETE) {
            std::cerr << "The framebuffer is not complete" << std::endl;
            return 0;
        }

        glCheckError(glBindFramebuffer(GL_FRAMEBUFFER, 0));
        return id;
    }

    /// 
    /// @brief Destroy the framebuffer
    /// @param id Framebuffer id
    /// 
    inline static void framebufferDestroy(PlatformId id) {
        glCheckError(glDeleteFramebuffers(1, &id));
    }

    /// 
    /// @brief Bind the framebuffer
    /// @param id Framebuffer id
    ///
    inline static void framebufferUse(PlatformId id) {
        glCheckError(glBindFramebuffer(GL_FRAMEBUFFER, id));
    }

    ///
    /// @brief Create vertex array
    /// @return vertex array id
    ///
    inline static PlatformId vertexArrayNew() {
        PlatformId id;
        glCheckError(glGenVertexArrays(1, &id));
        return id;
    }

    ///
    /// @brief Destroy vertex array
    /// @param id vertex array id
    ///
    inline static void vertexArrayDestroy(PlatformId id) {
        glCheckError(glDeleteVertexArrays(1, &id));
    }

    ///
    /// @brief Bind vertex array
    /// @param id vertex array id
    ///
    inline static void vertexArrayUse(PlatformId id) {
        glCheckError(glBindVertexArray(id));
    }

    ///
    /// @brief Create buffer
    /// @return buffer id
    ///
    inline static PlatformId bufferNew() {
        PlatformId id;
        glCheckError(glGenBuffers(1, &id));
        return id;
    }

    ///
    /// @brief Destroy buffer
    /// @param id buffer id
    ///
    inline static void bufferDestroy(PlatformId id) {
        glCheckError(glDeleteBuffers(1, &id));
    }

    ///
    /// @brief Bind buffer
    /// @param id buffer id
    /// @param mode buffer target mode
    /// @return buffer id
    ///
    inline static PlatformId bufferUse(PlatformId id, BufferMode mode) {
        glCheckError(glBindBuffer((GLenum)mode, id));
        return id;
    }

    ///
    /// @brief Set buffer data
    /// @param mode buffer target mode
    /// @param size buffer size
    /// @param data data
    /// @param target buffer data target mode
    ///
    inline static void bufferData(BufferMode mode, size_t size, const void* data, BufferTarget target) {
        glCheckError(glBufferData((GLenum)mode, (GLsizeiptr)size, data, (GLenum)target));
    }

    ///
    /// @brief VertexAttribArray
    /// @param index Specifies the index of the generic vertex attribute to be modified
    /// @param size Specifies the number of components per generic vertex attribute
    /// @param type Specifies the data type of each component in the array
    /// @param stride Specifies the byte offset between consecutive generic vertex attributes
    /// @param ptr Specifies a offset of the first component of the first generic vertex attribute
    ///
    inline static void vertexAttribArray(
        PlatformId index, 
        int size,
        AttribType type,
        size_t stride,
        const void* ptr) 
    {
        glCheckError(glEnableVertexAttribArray(index));
        glCheckError(glVertexAttribPointer(index, size, (GLenum)type, GL_FALSE, (GLsizei)stride, ptr));
    }

    ///
    /// @brief Draw arrays call
    /// @param mode Draw mode
    /// @param first Specifies the starting index in the enabled arrays
    /// @param count Specifies the number of indices to be rendered
    inline static void drawArrays(DrawMode mode, int first, size_t count) {
        glCheckError(glDrawArrays((GLenum)mode, first, (GLsizei)count));
    }

    ///
    /// @brief Draw elements call
    /// @param mode Specifies what kind of primitives to render
    /// @param count Specifies the number of elements to be rendered
    /// @param type Specifies the type of the values in indices
    /// @param indices Specifies a pointer to the location where the indices are stored
    ///
    inline static void drawElements(DrawMode mode, size_t count, AttribType type, const void* indices) {
        glCheckError(glDrawElements((GLenum)mode, (GLsizei)count, (GLenum)type, indices));
    }
};

#undef glCheckError

using Platform = OpenGL;
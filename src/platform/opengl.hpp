#pragma once

#include <GLFW/glfw3.h>
#include <glad.h>
#include <string>
#include <sstream>

using PlatformId = GLuint;

struct OpenGL {
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
    /// @brief Create a new shader program
    /// @return Shader program id
    /// 
    inline static PlatformId createProgram() {
        return glCreateProgram();
    }

    ///
    /// @brief Get program log
    /// @param id program id
    /// @return log
    /// 
    inline static const std::string getProgramLog(PlatformId id) {
        GLsizei length;
        std::string log;
        glGetProgramInfoLog(id, 1, &length, nullptr);
        log.reserve(length);
        glGetProgramInfoLog(id, 1, nullptr, (GLchar*)&log);
        return log;
    }

    ///
    /// @brief Attach shader to the program
    /// @param pid Program id
    /// @param sid Shader id
    /// @return program id
    ///
    inline static PlatformId attachShader(PlatformId pid, PlatformId sid) {
        glAttachShader(pid, sid);
        return pid;
    }

    ///
    /// @brief Detach shader to the program
    /// @param pid Program id
    /// @param sid Shader id
    /// @return program id
    ///
    inline static PlatformId detachShader(PlatformId pid, PlatformId sid) {
        glDetachShader(pid, sid);
        return pid;
    }

    ///
    /// @brief Link program
    /// @param id Program id
    /// @return program id
    ///
    inline static PlatformId linkProgram(PlatformId id) {
        glLinkProgram(id);
        return id;
    }

    ///
    /// @brief Destroy program
    /// @param id Program id
    ///
    inline static void destroyProgram(PlatformId id) {
        glDeleteProgram(id);
    }

    ///
    /// @brief Uniform1f
    /// @param id Program id
    /// @param name Uniform name
    /// @param value Value
    ///
    inline static PlatformId uniform1f(PlatformId id, const std::string& name, float value) {
        GLuint loc = glGetUniformLocation(id, name.c_str());
        glUniform1f(loc, value);
        return id;
    }

    ///
    /// @brief Uniform1i
    /// @param id Program id
    /// @param name Uniform name
    /// @param value Value
    ///
    inline static PlatformId uniform1i(PlatformId id, const std::string& name, int value) {
        GLuint loc = glGetUniformLocation(id, name.c_str());
        glUniform1i(loc, value);
        return id;
    }

    ///
    /// @brief Uniform3fv
    /// @param id Program id
    /// @param name Uniform name
    /// @param value Value
    ///
    inline static PlatformId uniform3fv(PlatformId id, const std::string& name, float value[3]) {
        GLuint loc = glGetUniformLocation(id, name.c_str());
        glUniform3fv(loc, 1, value);
        return id;
    }

    ///
    /// @brief Uniform4fv
    /// @param id Program id
    /// @param name Uniform name
    /// @param value Value
    ///
    inline static PlatformId uniform4fv(PlatformId id, const std::string& name, float value[4]) {
        GLuint loc = glGetUniformLocation(id, name.c_str());
        glUniform4fv(loc, 1, value);
        return id;
    }

    ///
    /// @brief UniformMatrix4fv
    /// @param id Program id
    /// @param name Uniform name
    /// @param value Value
    /// @param transpose Transpose matrix
    ///
    inline static PlatformId uniformMatrix4fv(PlatformId id, const std::string& name, float value[16], bool transpose = false) {
        GLuint loc = glGetUniformLocation(id, name.c_str());
        glUniformMatrix4fv(loc, 1, transpose, value);
        return id;
    }

    ///
    /// @brief Create a new vertex shader
    /// @return Shader id
    /// 
    inline static PlatformId createVertexShader() {
        return glCreateShader(GL_VERTEX_SHADER);
    }

    ///
    /// @brief Create a new fragment shader
    /// @return Shader id
    /// 
    inline static PlatformId createFragmentShader() {
        return glCreateShader(GL_FRAGMENT_SHADER);
    }

    ///
    /// @brief Destroy shader
    /// @param id Shader id
    ///
    inline static void destroyShader(PlatformId id) {
        glDeleteShader(id);
    }

    ///
    /// @brief Set shader source
    /// @param id shader id
    /// @param src shader source
    /// @return shader id
    ///
    inline static PlatformId shaderSource(PlatformId id, const std::string& src) {
        const GLchar* c_src = (const GLchar*)src.c_str();
        glShaderSource(id, 1, &c_src, nullptr);
        return id;
    }

    ///
    /// @brief Compile shader
    /// @param id shader id
    /// @return shader id
    ///
    inline static PlatformId compileShader(PlatformId id) {
        glCompileShader(id);
        return id;
    }

    ///
    /// @brief Get shader log
    /// @param id shader id
    /// @return log
    ///
    inline static const std::string getShaderLog(PlatformId id) {
        GLsizei length;
        std::string log;
        glGetShaderInfoLog(id, 1, &length, nullptr);
        log.reserve(length);
        glGetShaderInfoLog(id, 1, nullptr, (GLchar*)&log);
        return log;
    }

    ///
    /// @brief Bind program
    /// @param id program id
    /// @return program id
    ///
    inline static PlatformId useProgram(PlatformId id) {
        glUseProgram(id);
        return id;
    }

    ///
    /// @brief Create a new texture
    /// @return texture id
    ///
    inline static PlatformId createTexture() {
        PlatformId id;
        glGenTextures(1, &id);
        return id;
    }

    ///
    /// @brief Texture MIN filter
    /// @param id texture id
    /// @param filter filter parameter
    /// @return texture id
    ///
    inline static PlatformId textureMinParameter(PlatformId id, TextureFiltering filter) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)filter);
        return id;
    }

    ///
    /// @brief Texture MAG filter
    /// @param id texture id
    /// @param filter filter parameter
    /// @return texture id
    ///
    inline static PlatformId textureMagParameter(PlatformId id, TextureFiltering filter) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)filter);
        return id;
    }

    ///
    /// @brief Texture wrap S
    /// @param id texture id
    /// @param wrap wrap parameter
    /// @return texture id
    ///
    inline static PlatformId textureWrapSParameter(PlatformId id, TextureWrap wrap) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint)wrap);
        return id;
    }

    ///
    /// @brief Texture wrap T
    /// @param id texture id
    /// @param wrap wrap parameter
    /// @return texture id
    ///
    inline static PlatformId textureWrapTParameter(PlatformId id, TextureWrap wrap) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint)wrap);
        return id;
    }

    ///
    /// @brief Bind texture
    /// @param id texture id
    /// @return texture id
    ///
    inline static PlatformId textureBind(PlatformId id) {
        glBindTexture(GL_TEXTURE_2D, id);
        return id;
    }

    ///
    /// @brief Active texture at slot (slot)
    /// @param slot texture slot
    ///
    inline static void textureActiveUnit(unsigned char slot) {
        glActiveTexture(GL_TEXTURE0 + slot);
    }
};
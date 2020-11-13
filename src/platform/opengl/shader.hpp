#pragma once

#include "helper.hpp"
#include <string>
#include <vector>

class ShaderProgram {
    GLuint id;

public:
    ///
    /// @brief Constructor
    /// @param vertex Vertex shader source
    /// @param fragment Fragment shader source
    ///
    explicit ShaderProgram(const std::string& vertex, const std::string& fragment) : id(0) {
        GLuint vid, fid;
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
    }

    ///
    /// @brief Destructor
    ///
    ~ShaderProgram() { 
        glCheckError(glDeleteProgram(id)); 
    }

    ///
    /// @brief Use shader program
    ///
    inline void use() const { glCheckError(glUseProgram(id)); }

    ///
    /// @brief Uniform
    /// @param name Uniform name
    /// @param value Uniform value
    ///
    inline void uniform1i(const std::string& name, GLint value) const {
        GLint loc;
        glCheckError(loc = glGetUniformLocation(id, name.c_str()));
        glCheckError(glUniform1i(loc, value));
    }

    ///
    /// @brief Uniform
    /// @param name Uniform name
    /// @param value Uniform value
    ///
    inline void uniform1f(const std::string& name, float value) const {
        GLint loc;
        glCheckError(loc = glGetUniformLocation(id, name.c_str()));
        glCheckError(glUniform1f(loc, value));
    }

    ///
    /// @brief Uniform
    /// @param name Uniform name
    /// @param value Uniform value
    ///
    inline void uniform3f(const std::string& name, float value[3]) const {
        GLint loc;
        glCheckError(loc = glGetUniformLocation(id, name.c_str()));
        glCheckError(glUniform3fv(loc, 1, &value[0]));
    }

    ///
    /// @brief Uniform
    /// @param name Uniform name
    /// @param value Uniform value
    ///
    inline void uniform4f(const std::string& name, float value[4]) const {
        GLint loc;
        glCheckError(loc = glGetUniformLocation(id, name.c_str()));
        glCheckError(glUniform4fv(loc, 1, &value[0]));
    }

    ///
    /// @brief Uniform
    /// @param name Uniform name
    /// @param value Uniform value
    /// @param transpose True transpose matrix false otherwise
    ///
    inline void uniformMatrix(const std::string& name, float value[16], bool transpose = false) const {
        GLint loc;
        glCheckError(loc = glGetUniformLocation(id, name.c_str()));
        glCheckError(glUniformMatrix4fv(loc, 1, transpose, value));
    }

private:
    ///
    /// @brief Get program log
    /// @param pid program id
    /// @return log
    /// 
    const std::string getProgramLog(GLuint pid) {
        GLint length;
        glCheckError(glGetProgramiv(pid, GL_INFO_LOG_LENGTH, &length));
        std::vector<GLchar> log(length);
        glCheckError(glGetProgramInfoLog(pid, length, nullptr, log.data()));
        return std::string(log.begin(), log.end());
    }

    ///
    /// @brief Get shader log
    /// @param sid shader id
    /// @return log
    ///
    const std::string getShaderLog(GLuint sid) {
        GLint length;
        glCheckError(glGetShaderiv(sid, GL_INFO_LOG_LENGTH, &length));
        std::vector<GLchar> log(length);
        glCheckError(glGetShaderInfoLog(sid, length, nullptr, log.data()));
        return std::string(log.begin(), log.end());
    }
};
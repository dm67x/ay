#pragma once

#include "types.hpp"
#include "color.hpp"
#include <glad.h>
#include <string>
#include <glm/glm.hpp>
#include <spdlog/spdlog.h>
#include <vector>
#include <cassert>
#include <memory>
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

namespace ay
{
    class Shader
    {
    public:
        /// 
        /// @brief Create a new shader
        /// 
        Shader()
            : id(0)
        {
            glCheckError(id = glCreateProgram());
        }

        /// 
        /// @brief Destroy the shader
        ///
        ~Shader()
        {
            glCheckError(glDeleteProgram(id));
        }

        /// 
        /// @brief Load from memory
        /// @param vertex Vertex string
        /// @param fragment Fragment string
        /// @return True if shader loaded correctly false otherwise
        ///
        bool LoadFromMemory(const std::string& vertex, const std::string& fragment) const;

        /// 
        /// @brief Load from file
        /// @param vertex Vertex filename
        /// @param fragment Fragment filename
        /// @return True if shader loaded correctly false otherwise
        ///
        bool LoadFromFile(const std::string& vertex, const std::string& fragment) const;

        /// 
        /// @brief Use shader
        ///
        inline void Use() const
        {
            glCheckError(glUseProgram(id));
        }

        /// 
        /// @brief Set uniform value
        /// @param name Name of uniform
        /// @param value Value
        /// @return Shader instance
        ///
        inline const Shader& operator()(const std::string& name, f32 value) const 
        {
            GLint location;
            glCheckError(location = glGetUniformLocation(id, name.c_str()));
            glCheckError(glUniform1f(location, value));
            return *this;
        }

        /// 
        /// @brief Set uniform value
        /// @param name Name of uniform
        /// @param value Value
        /// @return Shader instance
        ///
        inline const Shader& operator()(const std::string& name, i32 value) const
        {
            GLint location;
            glCheckError(location = glGetUniformLocation(id, name.c_str()));
            glCheckError(glUniform1i(location, value));
            return *this;
        }

        /// 
        /// @brief Set uniform value
        /// @param name Name of uniform
        /// @param value Value
        /// @return Shader instance
        ///
        inline const Shader& operator()(const std::string& name, u32 value) const
        {
            GLint location;
            glCheckError(location = glGetUniformLocation(id, name.c_str()));
            glCheckError(glUniform1ui(location, value));
            return *this;
        }

        /// 
        /// @brief Set uniform value
        /// @param name Name of uniform
        /// @param value Value
        /// @return Shader instance
        ///
        inline const Shader& operator()(const std::string& name, const glm::vec2& value) const
        {
            GLint location;
            glCheckError(location = glGetUniformLocation(id, name.c_str()));
            glCheckError(glUniform2fv(location, 1, &value[0]));
            return *this;
        }

        /// 
        /// @brief Set uniform value
        /// @param name Name of uniform
        /// @param value Value
        /// @return Shader instance
        ///
        inline const Shader& operator()(const std::string& name, const glm::vec3& value) const
        {
            GLint location;
            glCheckError(location = glGetUniformLocation(id, name.c_str()));
            glCheckError(glUniform3fv(location, 1, &value[0]));
            return *this;
        }

        /// 
        /// @brief Set uniform value
        /// @param name Name of uniform
        /// @param value Value
        /// @return Shader instance
        ///
        inline const Shader& operator()(const std::string& name, const glm::vec4& value) const
        {
            GLint location;
            glCheckError(location = glGetUniformLocation(id, name.c_str()));
            glCheckError(glUniform4fv(location, 1, &value[0]));
            return *this;
        }

        /// 
        /// @brief Set uniform value
        /// @param name Name of uniform
        /// @param value Value
        /// @return Shader instance
        ///
        inline const Shader& operator()(const std::string& name, const glm::mat4& value) const
        {
            GLint location;
            glCheckError(location = glGetUniformLocation(id, name.c_str()));
            glCheckError(glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]));
            return *this;
        }

    private:
        GLuint id;
    };

    enum class AttributeIndex
    {
        POSITION = 0,
        NORMAL = 1,
        TEXCOORD_0 = 2,
        COLOR = 3
    };

    class Buffer
    {
    public:
        /// 
        /// @brief Create a new buffer
        /// 
        Buffer(GLenum target, u64 size, i32 offset = 0)
            : id(0),
            target(target),
            size(size),
            offset(offset),
            binded(false)
        {
            glCheckError(glGenBuffers(1, &id));
            (*this)();
            glCheckError(glBufferData(target, (GLsizeiptr)size, nullptr, GL_STATIC_DRAW));
            (*this)(false);
        }

        /// 
        /// @brief Destroy the buffer
        ///
        ~Buffer()
        {
            glCheckError(glDeleteBuffers(1, &id));
        }

        /// 
        /// @brief Bind (Unbind) the buffer
        /// 
        inline void operator()(bool bind = true)
        {
            if (bind && !binded) {
                glCheckError(glBindBuffer(target, id));
                binded = true;
            }
            else if (!bind && binded) {
                glCheckError(glBindBuffer(target, 0));
                binded = false;
            }
        }

        /// 
        /// @brief Set the buffer data
        /// @return Data pointer
        /// 
        inline GLvoid* SetData()
        {
            (*this)();
            GLvoid* data;
            glCheckError(data = glMapBufferRange(target, (GLintptr)offset, (GLsizeiptr)size, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT));
            return data;
        }

        /// 
        /// @brief Get the buffer data
        /// @return Data pointer
        /// 
        inline const GLvoid* GetData()
        {
            (*this)();
            GLvoid* data;
            glCheckError(data = glMapBufferRange(target, (GLintptr)offset, (GLsizeiptr)size, GL_MAP_READ_BIT));
            return (const GLvoid*)data;
        }

        ///
        /// @brief Set vertex attribute
        /// @param index Index of vertex attribute
        /// @param componentType Component size
        /// @param count Number of elements (stride)
        ///
        inline void SetAttrib(AttributeIndex index, u32 componentType, u32 count)
        {
            (*this)();
            glEnableVertexAttribArray((GLuint)index);
            glVertexAttribPointer((GLuint)index, componentType, GL_FLOAT, GL_FALSE, sizeof(f32) * count, nullptr);
        }

        ///
        /// @brief Unmap buffer
        /// 
        inline void Unmap()
        {
            if (binded) {
                glCheckError(glUnmapBuffer(target));
                glCheckError(glBindBuffer(target, 0));
                binded = false;
            }
        }

    private:
        friend class MeshBuffer;

    private:
        GLuint id;
        GLenum target;
        u64 size;
        i32 offset;
        bool binded;
    };

    class MeshBuffer
    {
    public:
        /// 
        /// @brief Create a new mesh buffer
        /// 
        MeshBuffer()
            : vao(0),
            buffers()
        {
            glCheckError(glGenVertexArrays(1, &vao));
        }

        /// 
        /// @brief Destroy the mesh buffer
        ///
        ~MeshBuffer()
        {
            glCheckError(glDeleteVertexArrays(1, &vao));
        }

        /// 
        /// @brief Bind vertex array
        /// 
        inline void BindVertexArray() const
        {
            glCheckError(glBindVertexArray(vao));
        }

        /// 
        /// @brief Unbind vertex array
        /// 
        static void UnbindVertexArray()
        {
            glCheckError(glBindVertexArray(0));
        }

        /// 
        /// @brief Create a new buffer 
        /// @param target Buffer target
        /// @param size Buffer length
        /// @param offset Buffer offset
        /// 
        inline void CreateBuffer(GLenum target, u64 size, i32 offset = 0)
        {
            buffers.push_back(std::make_shared<Buffer>(target, size, offset));
        }

        /// 
        /// @brief Get the buffer at position index
        /// @param index Index of the buffer
        ///
        inline std::shared_ptr<Buffer> operator[](i32 index) const
        {
            assert(index >= 0 && index < buffers.size());
            return buffers[index];
        }

        inline void Render() const
        {
            glBindVertexArray(vao);
            for (auto buffer : buffers) {
                if (buffer->target == GL_ELEMENT_ARRAY_BUFFER) {
                    (*buffer)();
                    glDrawElements(GL_TRIANGLES, buffer->size / sizeof(u32), GL_UNSIGNED_INT, nullptr);
                    (*buffer)(false);
                }
            }
            glBindVertexArray(0);
        }

    private:
        GLuint vao;
        std::vector<std::shared_ptr<Buffer>> buffers;
    };

    class Texture
    {
    public:
        ///
        /// @brief Create a new empty texture of size (1, 1)
        /// 
        Texture()
            : Texture(1, 1)
        {
        }

        ///
        /// @brief Create a new empty texture
        /// @param width Width
        /// @param height Height
        /// 
        Texture(u32 width, u32 height);

        ///
        /// @brief Destroy the texture
        /// 
        ~Texture()
        {
            glCheckError(glDeleteTextures(1, &id));
        }

        bool LoadFromFile(const std::string& filename) const;

        ///
        /// @brief Bind the texture to the selected slot
        /// @param slot Slot
        ///
        inline void operator[](i32 slot) const
        {
            if (slot < 0) {
                glCheckError(glBindTexture(GL_TEXTURE_2D, 0));
            }
            else {
                glCheckError(glActiveTexture(GL_TEXTURE0 + slot));
                glCheckError(glBindTexture(GL_TEXTURE_2D, id));
            }
        }

    private:
        GLuint id;
    };

    class GBuffer
    {

    };

    class RenderToTexture
    {

    };

    class Window;

    class Renderer
    {
    public:
        explicit Renderer(const Window& window);
        ~Renderer() = default;

        ///
        /// @brief Get OpenGL Version
        /// @return Version
        ///
        inline const std::string GetVersion() const 
        {
            std::stringstream version;
            version << glGetString(GL_VERSION);
            return version.str();
        }

        ///
        /// @brief Get Vendor
        /// @return Vendor
        ///
        inline const std::string GetVendor() const 
        {
            std::stringstream vendor;
            vendor << glGetString(GL_VENDOR);
            return vendor.str();
        }

        /// 
        /// @brief Get the attached window
        /// @return Window instance
        ///
        inline const Window& GetWindow() const
        {
            return window;
        }

        /// 
        /// @brief Get current shader
        /// @return Shader instance
        ///
        inline const std::shared_ptr<Shader> GetCurrentShader() const
        {
            return currentShader;
        }

        /// 
        /// @brief Clear the screen
        /// @param color Clear color
        ///
        inline void Viewport(i32 left, i32 top, i32 right, i32 bottom)
        {
            glCheckError(glViewport(left, top, right, bottom));
        }

        /// 
        /// @brief Clear the screen
        /// @param color Clear color
        ///
        inline void Clear(const Color& color)
        {
            glCheckError(glClearColor(color.r, color.g, color.b, color.a));
            glCheckError(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
        }

        void Begin();
        void Begin(const Shader& shader);
        void End();
        void BeginUI();
        void EndUI();

    private:
        const Window& window;
        std::shared_ptr<Shader> defaultShader;
        std::shared_ptr<Shader> currentShader;
        bool beginUI;
        bool begin;
    };
}
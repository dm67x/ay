#pragma once

#include "entity.hpp"
#include "rendering/vertex.hpp"
#include "rendering/transform.hpp"

#include <vector>
#include <string>
#include <glad/glad.h>
#include <cstdint>
#include <memory>

class Material;

class Mesh : public Entity
{
    class MeshPrimitive : public Transform
    {
        std::vector<Vertex> m_vertices;
        std::vector<GLuint> m_indices;
        GLuint m_vbo;
        GLuint m_vao;
        GLuint m_ebo;
        GLenum m_mode;
        std::shared_ptr<Material> m_material;

    public:
        MeshPrimitive();
        ~MeshPrimitive();

    public:
        Vertex& operator[](int);

    public:
        void add(const Vertex&);
        void add(GLuint);
        void set(GLenum);
        void set(const Material&);
        void build() const;
        void draw(const Shader&, const glm::mat4&) const;
    };

    
    std::vector<std::shared_ptr<MeshPrimitive>> m_primitives;

public:
    Mesh(const std::string&);
    ~Mesh() = default;

public:
    std::shared_ptr<MeshPrimitive>& create();
    void build();
    void draw(const Shader&, const glm::mat4& = glm::mat4(1)) const override;
};
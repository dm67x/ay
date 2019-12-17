#pragma once

#include "Scene/Entity.hpp"
#include "Ay.hpp"
#include "Render/Material.hpp"

#include <vector>
#include <glad/glad.h>

class Mesh;

class MeshRenderer : public SceneEntity
{
    const Mesh& m_mesh;
    std::vector<GLuint> m_indices;
    GLuint m_vbo;
    GLuint m_vao;
    GLuint m_ebo;
    Material* m_material;

public:
    AY_API MeshRenderer(const Mesh&, const std::string& = "default");
    AY_API ~MeshRenderer();
    AY_API void build();
    AY_API void draw(const ShaderProgram&) const override;
};
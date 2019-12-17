#pragma once

#include "Scene/Entity.hpp"
#include "Render/Material.hpp"

#include <vector>
#include <glad/glad.h>

class Mesh;

class Model : public SceneEntity
{
    std::vector<Mesh*> m_meshes;
    std::vector<Material*> m_materials;

private:
    bool loadMeshes(const std::string&);
    bool loadMaterials(const std::string&);

public:
    Model();
    ~Model();

public:
    void create(const std::vector<Mesh*>&, const std::vector<Material*>&);
    bool load(const std::string&);
    void build() const;
    void draw(const ShaderProgram&) const override;
};
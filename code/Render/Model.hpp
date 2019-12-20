#pragma once

#include "Scene/Entity.hpp"
#include "Render/Material.hpp"

#include <vector>
#include <glad/glad.h>
#include <memory>

class Mesh;

class Model : public SceneEntity
{
    std::vector<std::shared_ptr<Mesh>> m_meshes;
    std::vector<std::shared_ptr<Material>> m_materials;

private:
    bool loadMeshes(const std::string&);
    bool loadMaterials(const std::string&);

public:
    Model();

public:
    bool load(const std::string&);
    void build() const;
    void draw(const Shader&) const override;
};
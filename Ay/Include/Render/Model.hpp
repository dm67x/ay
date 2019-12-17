#pragma once

#include "Scene/Entity.hpp"
#include "Ay.hpp"
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
    AY_API Model();
    AY_API ~Model();

    AY_API void create(const std::vector<Mesh*>&, 
        const std::vector<Material*>&);
    AY_API bool load(const std::string&);
    AY_API void build() const;
    AY_API void draw(const ShaderProgram&) const override;
};
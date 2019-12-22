#pragma once

#include "entity/entity.hpp"

#include <tiny_gltf.h>
#include <memory>
#include <vector>

class Material;

class Scene
{
    std::string m_name;
    tinygltf::TinyGLTF m_loader;
    tinygltf::Model m_scene;
    std::vector<std::shared_ptr<Entity>> m_entities;
    std::vector<std::shared_ptr<Material>> m_materials;

private:
    void processNode(const tinygltf::Node&, int = -1);
    std::shared_ptr<Entity> processMesh(const tinygltf::Mesh&);
    std::shared_ptr<Entity> processCamera(const tinygltf::Camera&);

public:
    Scene(const std::string&);
    virtual ~Scene();

public:
    bool load(const std::string&);
    void draw(const Shader&) const;
};
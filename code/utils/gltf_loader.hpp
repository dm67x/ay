#pragma once

#include "entity/entity.hpp"

#include <tiny_gltf.h>
#include <memory>
#include <vector>
#include <glad/glad.h>

class Material;

class glTFLoader final
{
    std::string m_name;
    tinygltf::TinyGLTF m_loader;
    tinygltf::Model m_model;
    std::vector<std::shared_ptr<Entity>> m_entities;
    std::vector<std::shared_ptr<Material>> m_materials;

private:
    int accessorSize(const tinygltf::Accessor&) const;
    void processNode(const tinygltf::Node&, int = -1);
    std::shared_ptr<Entity> processMesh(const tinygltf::Mesh&);
    std::shared_ptr<Entity> processCamera(const tinygltf::Camera&);
    GLenum textureFilter(int) const;

public:
    glTFLoader();
    virtual ~glTFLoader() = default;

public:
    std::shared_ptr<Entity> operator[](int);

public:
    bool load(const std::string&);
    void draw(const Shader&) const;
    std::shared_ptr<Entity> get(const std::string&) const;
};
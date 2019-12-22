#include "scene.hpp"
#include "entity/camera.hpp"
#include "shader/shader.hpp"
#include "model/material.hpp"
#include "entity/mesh.hpp"

#include <iostream>

Scene::Scene(const std::string& name)
    : m_name{ name },
    m_loader{},
    m_scene{},
    m_entities{},
    m_materials{}
{
}

Scene::~Scene()
{
}

void Scene::processNode(const tinygltf::Node& node, int parent)
{
    std::shared_ptr<Entity> entity = nullptr;

    if (node.mesh > -1) {
        entity = processMesh(m_scene.meshes[node.mesh]);
    }
    /*else if (node.camera > -1) {
        entity = processCamera(m_scene.cameras[node.camera]);
    }*/
    else {
        //processLight();
    }

    if (entity) {
        if (node.scale.size() == 3) {
            entity->scale(glm::vec3(
                node.scale[0],
                node.scale[1],
                node.scale[2]));
        }

        if (node.translation.size() == 3) {
            entity->translate(glm::vec3(
                node.translation[0],
                node.translation[1],
                node.translation[2]));
        }

        if (node.rotation.size() == 4) {
            entity->rotate(node.rotation[0], glm::vec3(
                node.rotation[1],
                node.rotation[2],
                node.rotation[3]));
        }
    }

    auto it = std::find(m_scene.nodes.begin(), m_scene.nodes.end(), node);

    for (auto cn : node.children) {
        processNode(m_scene.nodes[cn], it - m_scene.nodes.begin());
    }
}

std::shared_ptr<Entity> Scene::processMesh(const tinygltf::Mesh& mesh)
{
    auto nmesh = std::make_shared<Mesh>();

    for (auto primitive : mesh.primitives) {
        // Accessors
        const tinygltf::Accessor& pAccessor = 
            m_scene.accessors[primitive.attributes["POSITION"]];
        const tinygltf::Accessor& nAccessor =
            m_scene.accessors[primitive.attributes["NORMAL"]];
        const tinygltf::Accessor& uvAccessor =
            m_scene.accessors[primitive.attributes["TEXCOORD_0"]];
        const tinygltf::Accessor& iAccessor =
            m_scene.accessors[primitive.indices];

        // Bufferviews
        const tinygltf::BufferView& pBufferView =
            m_scene.bufferViews[pAccessor.bufferView];
        const tinygltf::BufferView& nBufferView =
            m_scene.bufferViews[nAccessor.bufferView];
        const tinygltf::BufferView& uvBufferView =
            m_scene.bufferViews[uvAccessor.bufferView];
        const tinygltf::BufferView& iBufferView =
            m_scene.bufferViews[iAccessor.bufferView];

        // Buffers
        const tinygltf::Buffer& pBuffer = 
            m_scene.buffers[pBufferView.buffer];
        const tinygltf::Buffer& nBuffer = 
            m_scene.buffers[nBufferView.buffer];
        const tinygltf::Buffer& uvBuffer =
            m_scene.buffers[uvBufferView.buffer];
        const tinygltf::Buffer& iBuffer =
            m_scene.buffers[iBufferView.buffer];

        // Arrays
        const float* positions = reinterpret_cast<const float*>(
            &pBuffer.data[pBufferView.byteOffset + pAccessor.byteOffset]);
        const float* normals = reinterpret_cast<const float*>(
            &pBuffer.data[nBufferView.byteOffset + nAccessor.byteOffset]);
        const float* uvs = reinterpret_cast<const float*>(
            &pBuffer.data[uvBufferView.byteOffset + uvAccessor.byteOffset]);
        const unsigned char* indices = reinterpret_cast<const unsigned char*>(
            &iBuffer.data[iBufferView.byteOffset + iAccessor.byteOffset]);

        // Positions
        for (size_t i = 0; i < pAccessor.count; i++) {
            float x = positions[i * 3];
            float y = positions[i * 3 + 1];
            float z = positions[i * 3 + 2];

            Vertex vert;
            vert.position = glm::vec3(x, y, z);
            nmesh->m_vertices.push_back(vert);
        }

        // Normals
        for (size_t i = 0; i < nAccessor.count; i++) {
            float x = normals[i * 3];
            float y = normals[i * 3 + 1];
            float z = normals[i * 3 + 2];

            nmesh->m_vertices.at(i).normal = glm::vec3(x, y, z);
        }

        // UVs
        for (size_t i = 0; i < uvAccessor.count; i++) {
            float x = uvs[i * 3];
            float y = uvs[i * 3 + 1];

            nmesh->m_vertices.at(i).uv = glm::vec2(x, y);
        }

        // Indices
        const size_t indiceSize = tinygltf::GetComponentSizeInBytes(iAccessor.componentType);
        for (size_t i = 0; i < iAccessor.count; i++) {
            nmesh->m_indices.push_back(indices[i * indiceSize]);
        }
    }

    nmesh->build();
    m_entities.push_back(nmesh);
    return nmesh;
}

std::shared_ptr<Entity> Scene::processCamera(const tinygltf::Camera& camera)
{
    if (camera.type != "perspective" && camera.type != "orthographic") {
        std::cout << "camera type not supported" << std::endl;
        return nullptr;
    }

    auto cameraEntity = std::make_shared<Camera>();

    if (camera.type == "perspective") {
        auto cameraProjection = camera.perspective;
                
        glm::mat4 projection = glm::mat4();
        projection = glm::perspective(
            cameraProjection.yfov, 
            cameraProjection.aspectRatio, 
            cameraProjection.znear, 
            cameraProjection.zfar);

        cameraEntity->projection(projection);
    }
    else if (camera.type == "orthographic") {
        auto cameraProjection = camera.orthographic;

        glm::mat4 projection = glm::mat4();
        projection = glm::ortho(
            0.0,
            cameraProjection.xmag,
            cameraProjection.ymag,
            0.0,
            cameraProjection.znear,
            cameraProjection.zfar
        );

        cameraEntity->projection(projection);
    }

    m_entities.push_back(cameraEntity);
    return cameraEntity;
}

bool Scene::load(const std::string& filename)
{
    std::string err, warn;
    bool ret = m_loader.LoadBinaryFromFile(&m_scene, &err, &warn, filename);

    if (!warn.empty()) {
        std::cout << "warn: " << warn << std::endl;
    }

    if (!err.empty()) {
        std::cout << "err: " << err << std::endl;
    }

    if (!ret) {
        std::cout << "failed to parse glTF: " << filename << std::endl;
        return false;
    }

    for (auto material : m_scene.materials) {
        m_materials.push_back(std::make_shared<Material>(material.name));
    }

    auto currentScene = m_scene.scenes[0];
    for (auto node : currentScene.nodes) {
        processNode(m_scene.nodes[node]);
    }

    // Informations
    size_t length = 60 - m_name.size();
    for (size_t i = 0; i < length / 2; i++) std::cout << "=";
    std::cout << " " << m_name << " ";
    for (size_t i = 0; i < length / 2; i++) std::cout << "=";
    std::cout << std::endl;

    std::cout << m_entities.size() << " entities loaded" << std::endl;
    std::cout << m_materials.size() << " materials loaded" << std::endl;

    return true;
}

void Scene::draw(const Shader& shader) const
{
    for (auto entity : m_entities) {
        entity->draw(shader);
    }
}
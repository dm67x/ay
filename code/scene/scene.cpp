#include "scene.hpp"
#include "entity/camera.hpp"
#include "shader/shader.hpp"
#include "model/material.hpp"
#include "entity/mesh.hpp"
#include "model/texture2d.hpp"

#include <iostream>
#include <glm/gtx/matrix_decompose.hpp>

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

int Scene::accessorSize(const tinygltf::Accessor& accessor) const
{
    switch (accessor.type)
    {
    case TINYGLTF_TYPE_SCALAR:
        return 1;

    case TINYGLTF_TYPE_VEC2:
        return 2;

    case TINYGLTF_TYPE_VEC3:
        return 3;

    default:
        return -1;
    }
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
        auto pMatrix = glm::mat4(1);
            
        if (parent > -1) {
            auto parentNode = m_scene.nodes[parent];
            if (parentNode.matrix.size() == 16) {
                pMatrix = glm::mat4(
                    node.matrix[0], node.matrix[1], node.matrix[2], node.matrix[3],
                    node.matrix[4], node.matrix[5], node.matrix[6], node.matrix[7],
                    node.matrix[8], node.matrix[9], node.matrix[10], node.matrix[11],
                    node.matrix[12], node.matrix[13], node.matrix[14], node.matrix[15]
                );
            }
            else {
                glm::mat4 scale = glm::mat4(1);
                glm::mat4 rotate = glm::mat4(1);
                glm::mat4 translate = glm::mat4(1);

                if(parentNode.scale.size() == 3) {
                    scale = glm::scale(glm::mat4(1), glm::vec3(
                        (float)parentNode.scale[0],
                        (float)parentNode.scale[1],
                        (float)parentNode.scale[2]));
                }

                if (parentNode.translation.size() == 3) {
                    translate = glm::translate(glm::mat4(1), glm::vec3(
                        (float)parentNode.translation[0],
                        (float)parentNode.translation[1],
                        (float)parentNode.translation[2]));
                }

                /*if (parentNode.rotation.size() == 4) {
                    rotate = glm::rotate(glm::mat4(1),
                        (float)parentNode.rotation[0],
                        glm::vec3(
                            parentNode.rotation[1],
                            parentNode.rotation[2],
                            parentNode.rotation[3]));
                }*/

                pMatrix = scale * rotate * translate;
            }
        }

        if (node.matrix.size() == 16) {
            auto matrix = glm::mat4(
                node.matrix[0], node.matrix[1], node.matrix[2], node.matrix[3],
                node.matrix[4], node.matrix[5], node.matrix[6], node.matrix[7], 
                node.matrix[8], node.matrix[9], node.matrix[10], node.matrix[11], 
                node.matrix[12], node.matrix[13], node.matrix[14], node.matrix[15]
            );

            glm::vec3 translation;
            glm::vec3 scale;
            glm::quat rotation;
            glm::vec3 skew;
            glm::vec4 perspective;
            glm::decompose(matrix, scale, rotation, translation, skew, perspective);

            entity->translate(glm::vec3(pMatrix * glm::vec4(translation, 1.f)));
            entity->scale(glm::vec3(pMatrix * glm::vec4(scale, 1.f)));
        }
        else {
            if (node.scale.size() == 3) {
                glm::vec3 scale = glm::vec3(
                    node.scale[0],
                    node.scale[1],
                    node.scale[2]);
                    
                entity->scale(glm::vec3(pMatrix * glm::vec4(scale, 1.f)));
            }

            if (node.translation.size() == 3) {
                glm::vec3 translate = glm::vec3(
                    node.translation[0],
                    node.translation[1],
                    node.translation[2]);

                entity->translate(glm::vec3(pMatrix * glm::vec4(translate, 1.f)));
            }

            if (node.rotation.size() == 4) {
                glm::vec3 rotate = glm::vec3(
                    node.rotation[1],
                    node.rotation[2],
                    node.rotation[3]);
            }
        }
    }

    auto it = std::find(m_scene.nodes.begin(), m_scene.nodes.end(), node);
    for (auto cn : node.children) {
        processNode(m_scene.nodes[cn], (int)(it - m_scene.nodes.begin()));
    }
}

std::shared_ptr<Entity> Scene::processMesh(const tinygltf::Mesh& mesh)
{
    auto res = std::make_shared<Mesh>(mesh.name);

    for (auto primitive : mesh.primitives) {
        if (primitive.indices < 0) 
            continue;

        auto mprimitive = res->create();

        mprimitive->set(primitive.mode);
        if (primitive.material > -1) {
            mprimitive->set(*m_materials.at(primitive.material));
        }

        // Accessors
        auto pAccessor = m_scene.accessors[primitive.attributes["POSITION"]];
        auto nAccessor = m_scene.accessors[primitive.attributes["NORMAL"]];
        auto uvAccessor = m_scene.accessors[primitive.attributes["TEXCOORD_0"]];
        auto iAccessor = m_scene.accessors[primitive.indices];

        // Bufferviews
        auto pBufferView = m_scene.bufferViews[pAccessor.bufferView];
        auto nBufferView = m_scene.bufferViews[nAccessor.bufferView];
        auto uvBufferView = m_scene.bufferViews[uvAccessor.bufferView];
        auto iBufferView = m_scene.bufferViews[iAccessor.bufferView];

        // Buffers
        auto pBuffer = m_scene.buffers[pBufferView.buffer];
        auto nBuffer = m_scene.buffers[nBufferView.buffer];
        auto uvBuffer = m_scene.buffers[uvBufferView.buffer];
        auto iBuffer = m_scene.buffers[iBufferView.buffer];

        // Position inside buffers
        auto pPos = pBufferView.byteOffset + pAccessor.byteOffset;
        auto nPos = nBufferView.byteOffset + nAccessor.byteOffset;
        auto uvPos = uvBufferView.byteOffset + uvAccessor.byteOffset;
        auto iPos = iBufferView.byteOffset + iAccessor.byteOffset;

        // Arrays
        auto positions = reinterpret_cast<const float*>(&pBuffer.data[pPos]);
        auto normals = reinterpret_cast<const float*>(&nBuffer.data[nPos]);
        auto uvs = reinterpret_cast<const float*>(&uvBuffer.data[uvPos]);
        auto indices = reinterpret_cast<const unsigned short*>(&iBuffer.data[iPos]);

        // Sizes
        auto pSize = accessorSize(pAccessor);
        auto nSize = accessorSize(nAccessor);
        auto uvSize = accessorSize(uvAccessor);
        auto iSize = accessorSize(iAccessor);

        // Positions
        for (size_t i = 0; i < pAccessor.count; i++) {
            float x = positions[i * pSize];
            float y = positions[i * pSize + 1];
            float z = positions[i * pSize + 2];

            Vertex vert;
            vert.position = glm::vec3(x, y, z);
            mprimitive->add(vert);
        }

        // Normals
        for (size_t i = 0; i < nAccessor.count; i++) {
            float x = normals[i * nSize];
            float y = normals[i * nSize + 1];
            float z = normals[i * nSize + 2];

            (*mprimitive)[i].normal = glm::vec3(x, y, z);
        }

        // UVs
        for (size_t i = 0; i < uvAccessor.count; i++) {
            float x = uvs[i * uvSize];
            float y = uvs[i * uvSize + 1];

            (*mprimitive)[i].uv = glm::vec2(x, y);
        }

        // Indices
        for (size_t i = 0; i < iAccessor.count; i++) {
            mprimitive->add(indices[i * iSize]);
        }

        res->build();
    }

    m_entities.push_back(res);
    return res;
}

std::shared_ptr<Entity> Scene::processCamera(const tinygltf::Camera& camera)
{
    if (camera.type != "perspective") {
        std::cout << "camera type not supported" << std::endl;
        return nullptr;
    }

    auto entity = std::make_shared<Camera>(camera.name, 1.f);

    if (camera.type == "perspective") {
        auto projection = camera.perspective;
        entity->m_fov = (float)projection.yfov;
        entity->m_aspectRatio = (float)projection.aspectRatio;
        entity->m_zNear = (float)projection.znear;
        entity->m_zFar = (float)projection.zfar;
    }

    m_entities.push_back(entity);
    return entity;
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
        auto mat = std::make_shared<Material>(material.name);
        auto pbr = material.pbrMetallicRoughness;

        if (pbr.baseColorFactor.size() == 4) {
            mat->m_baseColorFactor = glm::vec4(
                pbr.baseColorFactor[0],
                pbr.baseColorFactor[1],
                pbr.baseColorFactor[2],
                pbr.baseColorFactor[3]);
        }

        mat->m_metallicFactor = (float)pbr.metallicFactor;
        mat->m_roughnessFactor = (float)pbr.roughnessFactor;

        if (pbr.baseColorTexture.index > -1) {
            auto texture = m_scene.textures[pbr.baseColorTexture.index];
            auto image = m_scene.images[texture.source];
            auto sampler = m_scene.samplers[texture.sampler];

            Texture2DParameter param;
            param.mag = sampler.magFilter;
            param.min = sampler.minFilter;
            param.wrap_s = sampler.wrapS;
            param.wrap_t = sampler.wrapT;

            auto tex = std::make_shared<Texture2D>(param);
            tex->load(image.uri);
            mat->m_baseColorTexture = tex;
        }

        if (pbr.metallicRoughnessTexture.index > -1) {
            auto texture = m_scene.textures[pbr.metallicRoughnessTexture.index];
            auto image = m_scene.images[texture.source];
            auto sampler = m_scene.samplers[texture.sampler];

            Texture2DParameter param;
            param.mag = sampler.magFilter;
            param.min = sampler.minFilter;
            param.wrap_s = sampler.wrapS;
            param.wrap_t = sampler.wrapT;

            auto tex = std::make_shared<Texture2D>(param);
            tex->load(image.uri);
            mat->m_metallicRoughnessTexture = tex;
        }

        if (material.normalTexture.index > -1) {
            auto texture = m_scene.textures[material.normalTexture.index];
            auto image = m_scene.images[texture.source];
            auto sampler = m_scene.samplers[texture.sampler];

            Texture2DParameter param;
            param.mag = sampler.magFilter;
            param.min = sampler.minFilter;
            param.wrap_s = sampler.wrapS;
            param.wrap_t = sampler.wrapT;

            auto tex = std::make_shared<Texture2D>(param);
            tex->load(image.uri);
            mat->m_normalTexture = tex;
        }

        if (material.emissiveFactor.size() == 3) {
            mat->m_emissiveFactor = glm::vec3(
                material.emissiveFactor[0],
                material.emissiveFactor[1],
                material.emissiveFactor[2]);
        }

        m_materials.push_back(mat);
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

std::shared_ptr<Entity> Scene::get(const std::string& name) const
{
    for (auto entity : m_entities) {
        if (entity->name() == name) {
            return entity;
        }
    }

    return nullptr;
}
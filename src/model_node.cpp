#include "model_node.hpp"
#include "model.hpp"
#include "material.hpp"
#include "mesh.hpp"
#include "context.hpp"
#include <glm/gtc/type_ptr.hpp>

ModelNode::~ModelNode() {
    for (auto mesh : meshes) {
        delete mesh;
    }

    for (auto child : children) {
        delete child;
    }
}

void ModelNode::render() const {
    for (size_t i = 0; i < meshes.size(); i++) {
        // default material
        Material* material = new Material();
        material->baseColor = Color::white();

        if (materials[i] >= 0) {
            delete material;
            material = model.materials.at(materials[i]);
        }

        model.ctx->shaderUniform("material.baseColor", material->baseColor.toVec());
        model.ctx->shaderUniform("material.metallicFactor", material->metallicFactor);
        model.ctx->shaderUniform("material.roughnessFactor", material->roughnessFactor);
        model.ctx->shaderUniform("material.emissiveFactor", material->emissiveFactor.toVec());

        auto _transform = getTransform();
        model.ctx->shaderUniform("modelMatrix", _transform);
        model.ctx->shaderUniform("normalMatrix", glm::transpose(glm::inverse(_transform)));

        meshes[i]->render();
    }

    for (auto child : children) {
        child->render();
    }
}

ModelNode* ModelNode::processNode(tinygltf::Model model, tinygltf::Node node) {
    auto translation = node.translation;
    auto rotation = node.rotation;
    auto scale = node.scale;
    auto matrix = node.matrix;

    ModelNode* _node = new ModelNode(*this);

    if (matrix.size() > 0) {
        _node->transform = Transform(glm::make_mat4(matrix.data()));
    }
    else {
        if (translation.size() > 0) {
            _node->transform.position = glm::vec3((f32)translation[0], (f32)translation[1], (f32)translation[2]);
        }
        
        if (scale.size() > 0) {
            _node->transform.scale = glm::vec3((f32)scale[0], (f32)scale[1], (f32)scale[2]);
        }

        if (rotation.size() > 0) {
            _node->transform.rotation = glm::quat(glm::vec3((f32)rotation[0], (f32)rotation[1], (f32)rotation[2]));
        }
    }

    if (node.mesh >= 0) {
        _node->processMesh(model, model.meshes[node.mesh]);
    }

    // if child process them
    for (auto childNode : node.children) {
        _node->addChild(processNode(model, model.nodes[childNode]));
    }

    return _node;
}

void ModelNode::processMesh(tinygltf::Model tmodel, tinygltf::Mesh tmesh) {
    for (auto primitive : tmesh.primitives) {
        Mesh* mesh = new Mesh(model.ctx);
        meshes.push_back(mesh);

        // Material
        i32 materialIndex = primitive.material;
        materials.push_back(materialIndex);
        if (materialIndex >= 0 && model.materials.find(materialIndex) == model.materials.end()) {
            auto material = tmodel.materials[materialIndex];
            auto pbr = material.pbrMetallicRoughness;
            Material* mat = new Material();
            mat->alphaCutoff = (f32)material.alphaCutoff;
            mat->baseColor = Color((f32)pbr.baseColorFactor[0], (f32)pbr.baseColorFactor[1], (f32)pbr.baseColorFactor[2], (f32)pbr.baseColorFactor[3]);
            mat->metallicFactor = (f32)pbr.metallicFactor;
            mat->emissiveFactor = Color((f32)material.emissiveFactor[0], (f32)material.emissiveFactor[1], (f32)material.emissiveFactor[2]);
            mat->doubleSided = material.doubleSided;
            mat->roughnessFactor = (f32)pbr.roughnessFactor;
            mat->name = material.name;
            model.materials.insert(std::make_pair(materialIndex, mat));
        }

        model.ctx->vaoUse(mesh->vao);

        // VBOs
        for (auto& attribute : primitive.attributes) {
            auto attrName = attribute.first;
            auto accessor = tmodel.accessors[attribute.second];
            i32 index = -1;
            index = (attrName == "POSITION") ? 0 : index;
            index = (attrName == "NORMAL") ? 1 : index;
            index = (attrName == "TEXCOORD_0") ? 2 : index;
            index = (attrName == "COLOR_0") ? 3 : index;

            if (index == -1) continue;
            auto bufferView = tmodel.bufferViews[accessor.bufferView];
            auto buffer = tmodel.buffers[bufferView.buffer];
            i32 stride = accessor.ByteStride(bufferView);

            model.ctx->bufferUse<BufferUsage::ARRAY>(mesh->buffers[(size_t)(index)+1]);

            GLsizeiptr bufferSize = 0;
            if (bufferView.byteLength + bufferView.byteOffset + accessor.byteOffset >= buffer.data.size()) {
                bufferSize = buffer.data.size() - (bufferView.byteOffset + accessor.byteOffset);
            }
            else {
                bufferSize = bufferView.byteLength;
            }

            model.ctx->bufferData<BufferUsage::ARRAY, BufferTarget::STATIC_DRAW>(bufferSize, buffer.data.data() + bufferView.byteOffset + accessor.byteOffset);
            model.ctx->bufferAttribute((Attribute)index, (GLenum)accessor.componentType, (GLint)accessor.type, (GLsizei)stride, nullptr);
            
            if (index == 0) {
                mesh->verticesCount = (u32)accessor.count;
            }
            else if (index == 1) {
                mesh->normalsCount = (u32)accessor.count;
            }
            else if (index == 2) {
                mesh->texcoordsCount = (u32)accessor.count;
            }
            else if (index == 3) {
                mesh->colorsCount = (u32)accessor.count;
            }
        }
        
        // EBO (indices)
        auto indexAccessor = tmodel.accessors[primitive.indices];
        auto indicesBufferView = tmodel.bufferViews[indexAccessor.bufferView];
        auto indicesBuffer = tmodel.buffers[indicesBufferView.buffer];
        model.ctx->bufferUse<BufferUsage::ELEMENT>(mesh->buffers[0]);
        model.ctx->bufferData<BufferUsage::ELEMENT, BufferTarget::STATIC_DRAW>(indicesBufferView.byteLength, indicesBuffer.data.data() + indicesBufferView.byteOffset);
        model.ctx->bufferUse<BufferUsage::ELEMENT>(0);
        mesh->indicesCount = (u32)indexAccessor.count;
        mesh->drawMode = (GLenum)primitive.mode;
        mesh->drawType = (GLenum)indexAccessor.componentType;

        model.ctx->vaoUse(0);
    }
}
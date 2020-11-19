#include "model.hpp"
#include <glm/gtc/type_ptr.hpp>

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

        meshes[i]->render(transform * parent->transform);
    }

    for (auto child : children) {
        child->render();
    }
}

Model::Model(Context* ctx)
    : Object(ctx),
    root(nullptr),
    materials()
{
    root = new ModelNode(*this);

    /*float axis[] = {
        0.f, 0.f, 0.f, 1.f, 0.f, 0.f,
        1.f, 0.f, 0.f, 1.f, 0.f, 0.f,
        0.f, 0.f, 0.f, 0.f, 1.f, 0.f,
        0.f, 1.f, 0.f, 0.f, 1.f, 0.f,
        0.f, 0.f, 0.f, 0.f, 0.f, 1.f,
        0.f, 0.f, 1.f, 0.f, 0.f, 1.f
    };

    ctx->vaoUse(axisVao);
    ctx->bufferUse<BufferUsage::ARRAY>(axisBuffer);
    ctx->bufferData<BufferUsage::ARRAY, BufferTarget::STATIC_DRAW>(sizeof(axis), &axis);
    ctx->bufferAttribute(0, GL_FLOAT, 3, 6 * sizeof(float), nullptr);
    ctx->bufferAttribute(3, GL_FLOAT, 3, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    ctx->bufferUse<BufferUsage::ARRAY>(0);
    ctx->vaoUse(0);*/
}

Model::~Model() {
    delete root;

    for (auto material : materials) {
        delete material.second;
    }
}

void Model::render(f32 deltaTime) {
    ctx->shaderUniform("isAxis", 0);
    root->transform = transform;
    root->render();
}

Model* Model::plane(Context* ctx) {
    Model* model = new Model(ctx);
    Mesh* mesh = new Mesh(ctx);

    std::vector<f32> data = {
        -1.f, 1.f, 0.f,
        1.f, 1.f, 0.f,
        1.f, -1.f, 0.f,
        -1.f, -1.f, 0.f,

        0.f, 0.f, 1.f,
        0.f, 0.f, 1.f,
        0.f, 0.f, 1.f,
        0.f, 0.f, 1.f,

        0.f, 0.f,
        1.f, 0.f,
        1.f, 1.f,
        0.f, 1.f,

        1.f, 1.f, 1.f,
        1.f, 1.f, 1.f,
        1.f, 1.f, 1.f,
        1.f, 1.f, 1.f
    };

    std::vector<u32> indices = {
        0, 2, 1, 
        0, 3, 2
    };

    mesh->indicesCount = (u32)indices.size();
    mesh->colorsCount = 4;
    mesh->normalsCount = 4;
    mesh->texcoordsCount = 4;
    mesh->verticesCount = 4;

    ctx->vaoUse(mesh->vao);
    ctx->bufferUse<BufferUsage::ARRAY>(mesh->buffers[1]);
    ctx->bufferData<BufferUsage::ARRAY, BufferTarget::STATIC_DRAW>(sizeof(f32) * 12, data.data());
    ctx->bufferAttribute(0, GL_FLOAT, 3, sizeof(f32) * 3, nullptr);

    ctx->bufferUse<BufferUsage::ARRAY>(mesh->buffers[2]);
    ctx->bufferData<BufferUsage::ARRAY, BufferTarget::STATIC_DRAW>(sizeof(f32) * 12, data.data() + (sizeof(f32) * 12));
    ctx->bufferAttribute(1, GL_FLOAT, 3, sizeof(f32) * 3, nullptr);

    ctx->bufferUse<BufferUsage::ARRAY>(mesh->buffers[3]);
    ctx->bufferData<BufferUsage::ARRAY, BufferTarget::STATIC_DRAW>(sizeof(f32) * 8, data.data() + (sizeof(f32) * 24));
    ctx->bufferAttribute(2, GL_FLOAT, 2, sizeof(f32) * 2, nullptr);

    ctx->bufferUse<BufferUsage::ARRAY>(mesh->buffers[4]);
    ctx->bufferData<BufferUsage::ARRAY, BufferTarget::STATIC_DRAW>(sizeof(f32) * 12, data.data() + (sizeof(f32) * 32));
    ctx->bufferAttribute(3, GL_FLOAT, 3, sizeof(f32) * 3, nullptr);

    ctx->bufferUse<BufferUsage::ELEMENT>(mesh->buffers[0]);
    ctx->bufferData<BufferUsage::ELEMENT, BufferTarget::STATIC_DRAW>(sizeof(u32) * indices.size(), indices.data());
    ctx->vaoUse(0);

    model->root->materials.push_back(-1);
    model->root->meshes.push_back(mesh);
    return model;
}

Model* Model::cube(Context* ctx) {
    Model* model = new Model(ctx);
    Mesh* mesh = new Mesh(ctx);

    std::vector<f32> data = {
        -1.f, 1.f, -1.f,
        -1.f, 1.f, 1.f,
        1.f, 1.f, 1.f,
        1.f, 1.f, -1.f,
        -1.f, -1.f, 1.f,
        -1.f, -1.f, -1.f,
        1.f, -1.f, -1.f,
        1.f, -1.f, 1.f,
        -1.f, 1.f, -1.f,
        -1.f, 1.f, -1.f,
        -1.f, 1.f, 1.f,
        -1.f, 1.f, 1.f,
        -1.f, -1.f, 1.f,
        -1.f, -1.f, -1.f,

        0.f, 0.f, 0.f,
        0.f, 0.f, 0.f,
        0.f, 0.f, 0.f,
        0.f, 0.f, 0.f,
        0.f, 0.f, 0.f,
        0.f, 0.f, 0.f,
        0.f, 0.f, 0.f,
        0.f, 0.f, 0.f,
        0.f, 0.f, 0.f,
        0.f, 0.f, 0.f,
        0.f, 0.f, 0.f,
        0.f, 0.f, 0.f,
        0.f, 0.f, 0.f,
        0.f, 0.f, 0.f,

        0.25f, 0.f,
        0.75f, 0.f,
        0.75f, 0.25f,
        0.25f, 0.25f,
        0.75f, 0.75f,
        0.25f, 0.75f,
        0.25f, 0.5f,
        0.75f, 0.5f,
        0.f, 0.25f,
        0.25f, 1.f,
        1.f, 0.25f,
        0.75f, 1.f,
        1.f, 0.5f,
        0.f, 0.5f,

        1.f, 1.f, 1.f,
        1.f, 1.f, 1.f,
        1.f, 1.f, 1.f,
        1.f, 1.f, 1.f,
        1.f, 1.f, 1.f,
        1.f, 1.f, 1.f,
        1.f, 1.f, 1.f,
        1.f, 1.f, 1.f,
        1.f, 1.f, 1.f,
        1.f, 1.f, 1.f,
        1.f, 1.f, 1.f,
        1.f, 1.f, 1.f,
        1.f, 1.f, 1.f,
        1.f, 1.f, 1.f
    };

    std::vector<u32> indices = {
        0, 2, 1, 0, 3, 2,
        2, 3, 6, 2, 6, 7,
        7, 6, 5, 7, 5, 4,
        3, 8, 13, 3, 13, 6,
        10, 2, 7, 10, 7, 12,
        4, 5, 9, 4, 9, 11
    };

    mesh->indicesCount = (u32)indices.size();
    mesh->colorsCount = 14;
    mesh->normalsCount = 14;
    mesh->texcoordsCount = 14;
    mesh->verticesCount = 14;

    ctx->vaoUse(mesh->vao);
    ctx->bufferUse<BufferUsage::ARRAY>(mesh->buffers[1]);
    ctx->bufferData<BufferUsage::ARRAY, BufferTarget::STATIC_DRAW>(sizeof(f32) * 42, data.data());
    ctx->bufferAttribute(0, GL_FLOAT, 3, sizeof(f32) * 3, nullptr);

    ctx->bufferUse<BufferUsage::ARRAY>(mesh->buffers[2]);
    ctx->bufferData<BufferUsage::ARRAY, BufferTarget::STATIC_DRAW>(sizeof(f32) * 42, data.data() + (sizeof(f32) * 42));
    ctx->bufferAttribute(1, GL_FLOAT, 3, sizeof(f32) * 3, nullptr);

    ctx->bufferUse<BufferUsage::ARRAY>(mesh->buffers[3]);
    ctx->bufferData<BufferUsage::ARRAY, BufferTarget::STATIC_DRAW>(sizeof(f32) * 28, data.data() + (sizeof(f32) * 84));
    ctx->bufferAttribute(2, GL_FLOAT, 2, sizeof(f32) * 2, nullptr);

    ctx->bufferUse<BufferUsage::ARRAY>(mesh->buffers[4]);
    ctx->bufferData<BufferUsage::ARRAY, BufferTarget::STATIC_DRAW>(sizeof(f32) * 42, data.data() + (sizeof(f32) * 112));
    ctx->bufferAttribute(3, GL_FLOAT, 3, sizeof(f32) * 3, nullptr);

    ctx->bufferUse<BufferUsage::ELEMENT>(mesh->buffers[0]);
    ctx->bufferData<BufferUsage::ELEMENT, BufferTarget::STATIC_DRAW>(sizeof(u32) * indices.size(), indices.data());
    ctx->vaoUse(0);

    mesh->computeNormals();

    model->root->materials.push_back(-1);
    model->root->meshes.push_back(mesh);
    return model;
}

Model* Model::fromFile(Context* ctx, const std::string& filename) {
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err, warn;
    Model* root = new Model(ctx);

    bool ret = loader.LoadBinaryFromFile(&model, &err, &warn, filename);
    if (!warn.empty()) {
        spdlog::warn("{}", warn);
    }

    if (!err.empty()) {
        spdlog::error("{}", err);
    }

    if (!ret) {
        spdlog::error("Failed to parse {}", filename);
        return root;
    }

    auto scene = model.scenes[model.defaultScene];
    for (auto node : scene.nodes) {
        root->root->addChild(root->processNode(model, model.nodes[node]));
    }

    spdlog::info("Mesh {} loaded successfully", filename);
    return root;
}

ModelNode* Model::processNode(tinygltf::Model model, tinygltf::Node node) {
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
            model.ctx->bufferData<BufferUsage::ARRAY, BufferTarget::STATIC_DRAW>(bufferView.byteLength, buffer.data.data() + bufferView.byteOffset);
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
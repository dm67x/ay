#include "model.hpp"
#include "context.hpp"
#include "model_node.hpp"
#include "mesh.hpp"
#include "material.hpp"

namespace ay
{
    Model::Model(Context* ctx)
        : ctx(ctx),
        root(nullptr),
        materials(),
        transform()
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
        Model* myModel = new Model(ctx);

        bool ret = loader.LoadBinaryFromFile(&model, &err, &warn, filename);
        if (!warn.empty()) {
            spdlog::warn("{}", warn);
        }

        if (!err.empty()) {
            spdlog::error("{}", err);
        }

        if (!ret) {
            spdlog::error("Failed to parse {}", filename);
            return myModel;
        }

        auto scene = model.scenes[model.defaultScene];
        for (auto node : scene.nodes) {
            myModel->root->addChild(myModel->root->processNode(model, model.nodes[node]));
        }

        spdlog::info("Mesh {} loaded successfully", filename);
        return myModel;
    }
}
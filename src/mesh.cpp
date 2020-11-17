#include "mesh.hpp"
#include "context.hpp"
#include <spdlog/spdlog.h>
#include <glm/gtc/type_ptr.hpp>

Mesh::Mesh(Context* ctx) 
    : Object(ctx), 
    vao(0), 
    ebo(0), 
    vbos(), 
    material(),
    drawMode(GL_TRIANGLES),
    drawType(GL_UNSIGNED_INT),
    indicesCount(0),
    axisVao(0),
    axisBuffer(0),
    isDebugMode(false),
    localTransformation(glm::mat4(1.f))
{
    vao = ctx->vaoNew();
    ebo = ctx->bufferNew();

    // create axis
    axisVao = ctx->vaoNew();
    axisBuffer = ctx->bufferNew();

    float axis[] = {
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
    ctx->vaoUse(0);
}

Mesh::~Mesh() {
    ctx->vaoDispose(vao);
    ctx->bufferDispose(ebo);
    ctx->bufferDispose(axisBuffer);

    for (auto vbo : vbos) {
        ctx->bufferDispose(vbo.second);
    }

    for (auto mesh : children) {
        delete mesh;
    }
}

Mesh* Mesh::plane(Context* ctx) {
    Mesh* plane = new Mesh(ctx);
    std::vector<Vertex> vertices = {
        Vertex(glm::vec3(-1.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(0.f, 0.f)),
        Vertex(glm::vec3(1.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(1.f, 0.f)),
        Vertex(glm::vec3(1.f, -1.f, 0.f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(1.f, 1.f)),
        Vertex(glm::vec3(-1.f, -1.f, 0.f), glm::vec3(0.f, 0.f, 1.f), glm::vec2(0.f, 1.f))
    };
    std::vector<unsigned int> indices = { 0, 2, 1, 0, 3, 2 };

    // Build
    ctx->vaoUse(plane->vao);
    Buffer vbo = ctx->bufferNew();
    plane->vbos.insert(std::make_pair(0, vbo));
    ctx->bufferUse<BufferUsage::ARRAY>(vbo);
    ctx->bufferData<BufferUsage::ARRAY, BufferTarget::STATIC_DRAW>((GLsizeiptr)(sizeof(Vertex) * vertices.size()), vertices.data());
    ctx->bufferAttribute(0, GL_FLOAT, (GLint)3, (GLsizei)sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
    ctx->bufferAttribute(1, GL_FLOAT, (GLint)3, (GLsizei)sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
    ctx->bufferAttribute(2, GL_FLOAT, (GLint)3, (GLsizei)sizeof(Vertex), (GLvoid*)offsetof(Vertex, uv));
    ctx->bufferUse<BufferUsage::ELEMENT>(plane->ebo);
    ctx->bufferData<BufferUsage::ELEMENT, BufferTarget::STATIC_DRAW>((GLsizeiptr)(sizeof(unsigned int) * indices.size()), indices.data());
    ctx->bufferUse<BufferUsage::ELEMENT>(0);
    ctx->vaoUse(0);

    plane->indicesCount = indices.size();
    return plane;
}

Mesh* Mesh::cube(Context* ctx) {
    Mesh* cube = new Mesh(ctx);
    std::vector<Vertex> vertices = {
        Vertex(glm::vec3(-1.f, 1.f, -1.f), glm::vec3(), glm::vec2(0.25f, 0.f)), // v1
        Vertex(glm::vec3(-1.f, 1.f, 1.f), glm::vec3(), glm::vec2(0.75f, 0.f)), // v2
        Vertex(glm::vec3(1.f, 1.f, 1.f), glm::vec3(), glm::vec2(0.75f, 0.25f)), // v3
        Vertex(glm::vec3(1.f, 1.f, -1.f), glm::vec3(), glm::vec2(0.25f, 0.25f)), // v4
        Vertex(glm::vec3(-1.f, -1.f, 1.f), glm::vec3(), glm::vec2(0.75f, 0.75f)), // v5
        Vertex(glm::vec3(-1.f, -1.f, -1.f), glm::vec3(), glm::vec2(0.25f, 0.75f)), // v6
        Vertex(glm::vec3(1.f, -1.f, -1.f), glm::vec3(), glm::vec2(0.25f, 0.5f)), // v7
        Vertex(glm::vec3(1.f, -1.f, 1.f), glm::vec3(), glm::vec2(0.75f, 0.5f)), // v8

        Vertex(glm::vec3(-1.f, 1.f, -1.f), glm::vec3(), glm::vec2(0.f, 0.25f)), // v12
        Vertex(glm::vec3(-1.f, 1.f, -1.f), glm::vec3(), glm::vec2(0.25f, 1.f)), // v13
        Vertex(glm::vec3(-1.f, 1.f, 1.f), glm::vec3(), glm::vec2(1.f, 0.25f)), // v22
        Vertex(glm::vec3(-1.f, 1.f, 1.f), glm::vec3(), glm::vec2(0.75f, 1.f)), // v23
        Vertex(glm::vec3(-1.f, -1.f, 1.f), glm::vec3(), glm::vec2(1.f, 0.5f)), // v52
        Vertex(glm::vec3(-1.f, -1.f, -1.f), glm::vec3(), glm::vec2(0.f, 0.5f)) // v62
    };
    std::vector<unsigned int> indices = {
        0, 2, 1, 0, 3, 2,
        2, 3, 6, 2, 6, 7,
        7, 6, 5, 7, 5, 4,
        3, 8, 13, 3, 13, 6,
        10, 2, 7, 10, 7, 12,
        4, 5, 9, 4, 9, 11
    };
    cube->computeNormals(vertices, indices, vertices);

    // Build
    ctx->vaoUse(cube->vao);
    Buffer vbo = ctx->bufferNew();
    cube->vbos.insert(std::make_pair(0, vbo));
    ctx->bufferUse<BufferUsage::ARRAY>(vbo);
    ctx->bufferData<BufferUsage::ARRAY, BufferTarget::STATIC_DRAW>((GLsizeiptr)(sizeof(Vertex) * vertices.size()), vertices.data());
    ctx->bufferAttribute(0, GL_FLOAT, (GLint)3, (GLsizei)sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
    ctx->bufferAttribute(1, GL_FLOAT, (GLint)3, (GLsizei)sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
    ctx->bufferAttribute(2, GL_FLOAT, (GLint)3, (GLsizei)sizeof(Vertex), (GLvoid*)offsetof(Vertex, uv));
    ctx->bufferUse<BufferUsage::ELEMENT>(cube->ebo);
    ctx->bufferData<BufferUsage::ELEMENT, BufferTarget::STATIC_DRAW>((GLsizeiptr)(sizeof(unsigned int) * indices.size()), indices.data());
    ctx->bufferUse<BufferUsage::ELEMENT>(0);
    ctx->vaoUse(0);

    cube->indicesCount = indices.size();
    return cube;
}

Mesh* Mesh::fromFile(Context* ctx, const std::string& filename) {
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err, warn;
    Mesh* root = new Mesh(ctx);

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
    for (auto nodeIndex : scene.nodes) {
        root->processNode(model, model.nodes[nodeIndex]);
    }

    spdlog::info("Mesh {} loaded successfully", filename);
    return root;
}

void Mesh::processNode(tinygltf::Model model, tinygltf::Node node) {
    auto translation = node.translation;
    auto rotation = node.rotation;
    auto scale = node.scale;
    auto matrix = node.matrix;

    Mesh* mesh = new Mesh(ctx);
    addChild(mesh);

    if (matrix.size() > 0) {
        std::vector<float> values;
        for (auto m : matrix) {
            values.push_back(static_cast<float>(m));
        }
        mesh->localTransformation = glm::make_mat4(values.data());
    }
    else {
        glm::mat4 translate = glm::mat4(1.f);
        glm::mat4 scaling = glm::mat4(1.f);
        glm::mat4 rotate = glm::mat4(1.f);

        if (translation.size() > 0) {
            glm::vec3 t((float)translation[0], (float)translation[1], (float)translation[2]);
            translate = glm::translate(glm::mat4(1.f), t);
        }

        if (scale.size() > 0) {
            glm::vec3 s((float)scale[0], (float)scale[1], (float)scale[2]);
            scaling = glm::scale(glm::mat4(1.f), s);
        }

        if (rotation.size() > 0) {
            glm::vec3 r(glm::radians((float)rotation[0]), glm::radians((float)rotation[1]), glm::radians((float)rotation[2]));
            rotate = glm::toMat4(glm::quat(r));
        }

        mesh->localTransformation = translate * rotate * scaling;
    }

    if (node.mesh >= 0) {
        mesh->processMesh(model, model.meshes[node.mesh]);
    }

    for (auto child : node.children) {
        processNode(model, model.nodes[child]);
    }
}

void Mesh::processMesh(tinygltf::Model model, tinygltf::Mesh mesh) {
    for (auto primitive : mesh.primitives) {
        Mesh* finalMesh = new Mesh(ctx);

        // Material
        int materialIndex = primitive.material;
        if (materialIndex >= 0) {
            auto mat = model.materials[materialIndex];
            finalMesh->processMaterial(model, mat);
        }

        ctx->vaoUse(finalMesh->vao);

        // VBOs
        for (auto& attribute : primitive.attributes) {
            auto accessor = model.accessors[attribute.second];
            auto bufferView = model.bufferViews[accessor.bufferView];
            auto buffer = model.buffers[bufferView.buffer];
            int stride = accessor.ByteStride(bufferView);

            if (bufferView.target != (int)BufferUsage::ARRAY) {
                continue;
            }

            int index = -1;
            if (attribute.first == "POSITION") {
                index = 0;
            }
            else if (attribute.first == "NORMAL") {
                index = 1;
            }
            else if (attribute.first == "TEXCOORD_0") {
                index = 2;
            }
            else if (attribute.first == "COLOR_0") {
                index = 3;
            }

            if (index >= 0) {
                auto it = finalMesh->vbos.find(index);
                if (it == finalMesh->vbos.end()) {
                    finalMesh->vbos.insert(std::make_pair(index, ctx->bufferNew()));
                    it = finalMesh->vbos.find(index);
                    ctx->bufferUse<BufferUsage::ARRAY>(it->second);
                    ctx->bufferData<BufferUsage::ARRAY, BufferTarget::STATIC_DRAW>((GLsizeiptr)(bufferView.byteLength), buffer.data.data() + bufferView.byteOffset);
                }

                ctx->bufferAttribute(index, (GLenum)accessor.componentType, (GLint)accessor.type, (GLsizei)stride, (GLvoid*)accessor.byteOffset);
            }
        }

        // EBO (indices)
        ctx->bufferUse<BufferUsage::ELEMENT>(finalMesh->ebo);
        auto accessor = model.accessors[primitive.indices];
        auto indicesBufferView = model.bufferViews[accessor.bufferView];
        auto indicesBuffer = model.buffers[indicesBufferView.buffer];
        ctx->bufferData<BufferUsage::ELEMENT, BufferTarget::STATIC_DRAW>((GLsizeiptr)(indicesBufferView.byteLength), indicesBuffer.data.data() + indicesBufferView.byteOffset);
        ctx->bufferUse<BufferUsage::ELEMENT>(0);
        ctx->vaoUse(0);

        finalMesh->drawMode = primitive.mode;
        finalMesh->indicesCount = accessor.count;
        finalMesh->drawType = accessor.componentType;
        addChild(finalMesh);
    }
}

void Mesh::processMaterial(tinygltf::Model model, tinygltf::Material mat) {
    (void)model;

    material.alphaCutoff = static_cast<float>(mat.alphaCutoff);
    auto baseColor = mat.pbrMetallicRoughness.baseColorFactor;
    material.baseColor = Color((float)baseColor[0], (float)baseColor[1], (float)baseColor[2], (float)baseColor[3]);
    material.metallicFactor = static_cast<float>(mat.pbrMetallicRoughness.metallicFactor);
    material.emissiveFactor = Color((float)mat.emissiveFactor[0], (float)mat.emissiveFactor[1], (float)mat.emissiveFactor[2]);
    material.doubleSided = mat.doubleSided;
    material.roughnessFactor = static_cast<float>(mat.pbrMetallicRoughness.roughnessFactor);
    material.name = mat.name;
}

void Mesh::render(float deltaTime) {
    (void)deltaTime;

    glm::mat4 _transform = getTransform();
    ctx->shaderUniform("modelMatrix", _transform);
    ctx->shaderUniform("normalMatrix", glm::transpose(glm::inverse(_transform)));
    ctx->shaderUniform("isAxis", 0);
    ctx->shaderUniform("material.baseColor", material.baseColor.toVec());
    ctx->shaderUniform("material.metallicFactor", material.metallicFactor);
    ctx->shaderUniform("material.roughnessFactor", material.roughnessFactor);
    ctx->shaderUniform("material.emissiveFactor", material.emissiveFactor.toVec());

    ctx->vaoUse(vao);
    ctx->bufferUse<BufferUsage::ELEMENT>(ebo);
    if (isDebugMode) {
        ctx->draw(DrawMethod::ELEMENT, DrawParameters(GL_LINES, (GLenum)drawType, (GLsizei)indicesCount, nullptr));
    }
    else {
        ctx->draw(DrawMethod::ELEMENT, DrawParameters((GLenum)drawMode, (GLenum)drawType, (GLsizei)indicesCount, nullptr));
    }
    ctx->bufferUse<BufferUsage::ELEMENT>(0);

    if (isDebugMode) {
        ctx->shaderUniform("modelMatrix", _transform * glm::scale(glm::mat4(1.f), glm::vec3(2.f, 2.f, 2.f)));
        ctx->vaoUse(axisVao);
        ctx->shaderUniform("isAxis", 1);
        glCheckError(glLineWidth(3.f));
        ctx->draw(DrawMethod::ARRAY, DrawParameters(GL_LINES, 0, 6));
        glCheckError(glLineWidth(1.f));
    }

    ctx->vaoUse(0);

    for (auto mesh : children) {
        mesh->render(deltaTime);
    }
}

void Mesh::computeNormals(
    const std::vector<Vertex>& vertices, 
    const std::vector<unsigned int>& indices, 
    std::vector<Vertex>& output) 
{
    output = vertices;

    // Get faces
    for (size_t i = 0; i < indices.size(); i += 3) {
        unsigned int v1i = indices[i];
        unsigned int v2i = indices[i + 1];
        unsigned int v3i = indices[i + 2];

        glm::vec3 v1 = vertices[v1i].position;
        glm::vec3 v2 = vertices[v2i].position;
        glm::vec3 v3 = vertices[v3i].position;

        glm::vec3 v1v2 = v2 - v1;
        glm::vec3 v1v3 = v3 - v1;

        glm::vec3 normal = glm::normalize(v1v2 * v1v3);

        output[v1i].normal = normal;
        output[v2i].normal = normal;
        output[v3i].normal = normal;
    }
}
#pragma once

#include "types.hpp"
#include "context.hpp"
#include "transform.hpp"
#include <string>
#include <array>
#include <vector>

class Mesh {
    friend class Model;
    friend class ModelNode;

    Context* ctx;
    Transform transform;
    VAO vao;
    std::array<Buffer, 5> buffers;
    u32 verticesCount;
    u32 normalsCount;
    u32 texcoordsCount;
    u32 colorsCount;
    u32 indicesCount;
    GLenum drawMode;
    GLenum drawType;

public:
    ///
    /// @brief Constructor
    /// @param ctx Context pointer
    ///
    Mesh(Context* ctx)
        : ctx(ctx),
        transform(),
        vao(0),
        buffers(),
        verticesCount(0),
        normalsCount(0),
        texcoordsCount(0),
        colorsCount(0),
        indicesCount(0),
        drawMode(GL_TRIANGLES),
        drawType(GL_UNSIGNED_INT)
    {
        vao = ctx->vaoNew();
        buffers[0] = ctx->bufferNew(); // indices
        buffers[1] = ctx->bufferNew(); // positions
        buffers[2] = ctx->bufferNew(); // normals
        buffers[3] = ctx->bufferNew(); // uvs
        buffers[4] = ctx->bufferNew(); // colors
    }

    ///
    /// @brief Destructor
    ///
    ~Mesh() {
        ctx->vaoDispose(vao);
        for (auto buffer : buffers) {
            ctx->bufferDispose(buffer);
        }
    }

    ///
    /// @brief Render called each frame
    ///
    inline void render(const Transform& parent) {
        auto _transform = getTransform() * parent.getTransform();
        ctx->shaderUniform("modelMatrix", _transform);
        ctx->shaderUniform("normalMatrix", glm::transpose(glm::inverse(_transform)));

        ctx->vaoUse(vao);
        ctx->bufferUse<BufferUsage::ELEMENT>(buffers[0]);
        ctx->draw(DrawMethod::ELEMENT, DrawParameters(drawMode, drawType, indicesCount, nullptr));
        ctx->bufferUse<BufferUsage::ELEMENT>(0);
        ctx->vaoUse(0);
    }

    ///
    /// @brief Get model matrix
    /// @return Mat4
    ///
    inline glm::mat4 getTransform() const {
        return transform.getTransform();
    }

    ///
    /// @brief Compute the normals
    /// 
    inline void computeNormals() {
        // Normals
        std::vector<f32> normals;
        normals.resize(3 * (size_t)normalsCount);

        // Indices
        ctx->bufferUse<BufferUsage::ELEMENT>(buffers[0]);
        u32* dataIndex = reinterpret_cast<u32*>(ctx->bufferGetSubData<BufferUsage::ELEMENT>(0, indicesCount * sizeof(u32)));

        // Positions
        ctx->bufferUse<BufferUsage::ARRAY>(buffers[1]);
        f32* dataPosition = reinterpret_cast<f32*>(ctx->bufferGetSubData<BufferUsage::ARRAY>(0, verticesCount * sizeof(f32) * 3));

        for (size_t i = 0; i < indicesCount; i += 3) {
            const size_t i1 = (size_t)dataIndex[i];
            const size_t i2 = (size_t)dataIndex[i + 1];
            const size_t i3 = (size_t)dataIndex[i + 2];

            const f32 x1 = dataPosition[i1 * 3];
            const f32 y1 = dataPosition[i1 * 3 + 1];
            const f32 z1 = dataPosition[i1 * 3 + 2];

            const f32 x2 = dataPosition[i2 * 3];
            const f32 y2 = dataPosition[i2 * 3 + 1];
            const f32 z2 = dataPosition[i2 * 3 + 2];

            const f32 x3 = dataPosition[i3 * 3];
            const f32 y3 = dataPosition[i3 * 3 + 1];
            const f32 z3 = dataPosition[i3 * 3 + 2];

            glm::vec3 v1 = glm::vec3(x1, y1, z1);
            glm::vec3 v2 = glm::vec3(x2, y2, z2);
            glm::vec3 v3 = glm::vec3(x3, y3, z3);
            glm::vec3 u = v2 - v1;
            glm::vec3 v = v3 - v1;
            glm::vec3 normal = glm::normalize(glm::cross(u, v));

            normals[i1 * 3] = normal.x;
            normals[i1 * 3 + 1] = normal.y;
            normals[i1 * 3 + 2] = normal.z;

            normals[i2 * 3] = normal.x;
            normals[i2 * 3 + 1] = normal.y;
            normals[i2 * 3 + 2] = normal.z;

            normals[i3 * 3] = normal.x;
            normals[i3 * 3 + 1] = normal.y;
            normals[i3 * 3 + 2] = normal.z;
        }

        // Clean all and save data
        ctx->bufferUnmap<BufferUsage::ARRAY>();
        ctx->bufferUse<BufferUsage::ARRAY>(buffers[2]);
        f32* ptr = reinterpret_cast<f32*>(ctx->bufferSetSubData<BufferUsage::ARRAY>(0, normalsCount * sizeof(f32) * 3));
        std::copy(normals.begin(), normals.end(), ptr);
        ctx->bufferUnmap<BufferUsage::ARRAY>();
        ctx->bufferUse<BufferUsage::ELEMENT>(buffers[0]);
        ctx->bufferUnmap<BufferUsage::ELEMENT>();
        ctx->bufferUse<BufferUsage::ELEMENT>(0);
    }
};
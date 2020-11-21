#pragma once

#include "types.hpp"
#include "transform.hpp"
#include "context.hpp"
#include "mesh.hpp"
#include "material.hpp"
#include "tiny_gltf.h"
#include <vector>
#include <map>

class ModelNode {
private:
    ///
    /// @brief Constructor
    /// @param model Model
    /// 
    ModelNode(Model& model) 
        : model(model), parent(nullptr), children(), transform(), meshes(), materials() 
    {
    }

    ///
    /// @brief Destructor
    /// 
    ~ModelNode() {
        for (auto mesh : meshes) {
            delete mesh;
        }

        for (auto child : children) {
            delete child;
        }
    }

    ///
    /// @brief Add child
    /// @param n Child node
    /// 
    inline void addChild(ModelNode* n) {
        children.push_back(n);
        n->parent = this;
    }

    ///
    /// @brief Get transform
    /// @return Transform
    ///
    inline glm::mat4 getTransform() const {
        glm::mat4 t = transform.getTransform(); // local transform
        if (parent != nullptr) {
            t *= parent->getTransform();
        }
        return t;
    }

    ///
    /// @brief Render
    /// 
    void render() const;

    ///
    /// @brief Process Node
    /// @param model glTF model
    /// @param node glTF node
    /// 
    ModelNode* processNode(tinygltf::Model model, tinygltf::Node node);

    ///
    /// @brief Process Mesh
    /// @param model glTF model
    /// @param mesh glTF mesh
    /// 
    void processMesh(tinygltf::Model tmodel, tinygltf::Mesh tmesh);

private:
    friend class Model;

private:
    Model& model;
    ModelNode* parent;
    std::vector<ModelNode*> children;
    Transform transform;
    std::vector<Mesh*> meshes;
    std::vector<i32> materials;
};

class Model {
public:
    ///
    /// @brief Destructor
    ///
    ~Model();

    ///
    /// @brief Create a new plane mesh
    /// @param ctx Context
    /// @return Plane mesh
    ///
    static Model* plane(Context* ctx);

    ///
    /// @brief Create a new cube mesh
    /// @param ctx Context
    /// @return Cube mesh
    ///
    static Model* cube(Context* ctx);

    ///
    /// @brief Create a new mesh from file (obj)
    /// @param ctx Context
    /// @param filename Filename
    /// @return Mesh
    ///
    static Model* fromFile(Context* ctx, const std::string& filename);

    ///
    /// @brief Render called each frame
    /// @param deltaTime Elapsed time between each frame
    ///
    void render(f32 deltaTime);

private:
    ///
    /// @brief Constructor
    /// @param ctx Context
    ///
    Model(Context* ctx);

private:
    friend class ModelNode;

private:
    Context* ctx;
    ModelNode* root;
    std::map<i32, Material*> materials;

public:
    Transform transform;
};
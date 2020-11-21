#pragma once

#include "types.hpp"
#include "tiny_gltf.h"
#include "transform.hpp"
#include "mesh.hpp"
#include <vector>

class Model;

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
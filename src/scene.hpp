#pragma once

#include "object.hpp"
#include "camera.hpp"
#include <map>
#include <string>

class Scene : public Object {
    std::map<std::string, Camera*> cameras;
    Camera* mainCamera;

protected:
    int width;
    int height;

public:
    ///
    /// @brief Create a new scene
    /// @param ctx The context
    ///
    Scene(Context* ctx, int width, int height) 
        : Object(ctx), cameras(), mainCamera(nullptr), width(width), height(height)
    {
    }

    ///
    /// @brief Destructor
    ///
    virtual ~Scene() override {
        for (auto camera : cameras) {
            delete camera.second;
        }
    }

    ///
    /// @brief Create a new perspective camera
    /// @param name Camera name
    /// @param fov FOV
    /// @param zNear zNear
    /// @param zFar zFar
    /// @return Created camera
    ///
    inline Camera* createPerspectiveCamera(const std::string& name, float fov, float zNear, float zFar) {
        Camera* camera = new PerspectiveCamera(ctx, fov, static_cast<float>(width) / static_cast<float>(height), zNear, zFar);
        cameras.insert(std::make_pair(name, camera));
        return camera;
    }

    ///
    /// @brief Create a new orthographic camera
    /// @param name Camera name
    /// @param left Left
    /// @param right Right
    /// @param bottom Bottom
    /// @param top Top
    /// @param zNear zNear
    /// @param zFar zFar
    /// @return Created camera
    ///
    inline Camera* createOrthographicCamera(const std::string& name, float left, float right, float bottom, float top, float zNear, float zFar) {
        Camera* camera = new OrthographicCamera(ctx, left, right, bottom, top, zNear, zFar);
        cameras.insert(std::make_pair(name, camera));
        return camera;
    }

    ///
    /// @brief Set main camera
    /// @param name Camera name
    ///
    inline void setMainCamera(const std::string& name) {
        auto it = cameras.find(name);
        if (it != cameras.end()) {
            mainCamera = it->second;
        }
    }

    ///
    /// @brief Update called each frame
    /// @param deltaTime Elapsed time between each frame
    ///
    virtual void update(float deltaTime) override = 0;

    ///
    /// @brief Render called each frame
    /// @param deltaTime Elapsed time between each frame
    ///
    virtual void render(float deltaTime) override {
        mainCamera->update(deltaTime);
    }
};
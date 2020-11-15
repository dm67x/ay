#pragma once

#include "object.hpp"
#include "camera.hpp"
#include "light.hpp"
#include <map>
#include <string>

class Scene : public Object {
    std::map<std::string, Camera*> cameras;
    Camera* mainCamera;
    std::array<Light*, 16> lights;
    size_t numberOfLights;

protected:
    int width;
    int height;

public:
    ///
    /// @brief Create a new scene
    /// @param ctx The context
    ///
    Scene(Context* ctx, int width, int height) 
        : Object(ctx), cameras(), mainCamera(nullptr), lights(), numberOfLights(0), width(width), height(height)
    {
    }

    ///
    /// @brief Destructor
    ///
    virtual ~Scene() override {
        for (auto camera : cameras) {
            delete camera.second;
        }

        for (auto light : lights) {
            delete light;
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
    /// @brief Create a new light source
    /// @return Light Light
    ///
    inline Light* createLight() {
        if (lights.size() <= numberOfLights) {
            spdlog::error("You created more than 16 lights");
            return nullptr;
        }

        Light* light = new Light();
        lights.at(numberOfLights++) = light;
        return light;
    }

    ///
    /// @brief Render called each frame
    /// @param deltaTime Elapsed time between each frame
    ///
    virtual void render(float deltaTime) override {
        mainCamera->update(deltaTime);

        // lights
        for (size_t i = 0; i < numberOfLights; i++) {
            auto light = lights[i];
            std::stringstream ss;
            ss << "lights[" << i << "]";
            ctx->shaderUniform(ss.str() + ".color", light->color.toVec());
            ctx->shaderUniform(ss.str() + ".position", light->position);
            ctx->shaderUniform(ss.str() + ".power", light->power);
        }
        ctx->shaderUniform("lightsCount", static_cast<int>(numberOfLights));
    }
};
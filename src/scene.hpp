#pragma once

#include "types.hpp"
#include "object.hpp"
#include "camera.hpp"
#include "light.hpp"
#include <map>
#include <string>
#include <functional>

class Scene : public Object {
    std::map<std::string, Camera*> cameras;
    Camera* mainCamera;
    std::array<Light*, 16> pointsLights;
    std::array<Light*, 8> directionalLights;
    size_t numberOfPointLights;
    size_t numberOfDirectionalLights;

public:
    std::function<void(Scene*, f32)> onRender;
    std::function<void(Scene*)> onDestroy;

public:
    ///
    /// @brief Create a new scene
    /// @param ctx The context
    ///
    Scene(Context* ctx) 
        : Object(ctx), 
        cameras(), 
        mainCamera(nullptr), 
        pointsLights(),
        directionalLights(),
        numberOfPointLights(0),
        numberOfDirectionalLights(0),
        onRender([](Scene*, f32) {}),
        onDestroy([](Scene*) {})
    {
    }

    ///
    /// @brief Destructor
    ///
    ~Scene() override {
        for (auto camera : cameras) {
            delete camera.second;
        }

        for (auto light : pointsLights) {
            delete light;
        }

        for (auto light : directionalLights) {
            delete light;
        }

        onDestroy(this);
    }

    ///
    /// @brief Create a new perspective camera
    /// @param name Camera name
    /// @param fov FOV
    /// @param zNear zNear
    /// @param zFar zFar
    /// @return Created camera
    ///
    inline Camera* createPerspectiveCamera(const std::string& name, f32 fov, f32 zNear, f32 zFar) {
        Camera* camera = new PerspectiveCamera(ctx, fov, zNear, zFar);
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
    inline Camera* createOrthographicCamera(const std::string& name, f32 left, f32 right, f32 bottom, f32 top, f32 zNear, f32 zFar) {
        Camera* camera = new OrthographicCamera(ctx, left, right, bottom, top, zNear, zFar);
        cameras.insert(std::make_pair(name, camera));
        return camera;
    }

    ///
    /// @brief Create a new free camera
    /// @param name Camera name
    /// @param fov FOV
    /// @param zNear zNear
    /// @param zFar zFar
    /// @return Created camera
    ///
    inline Camera* createFreeCamera(const std::string& name, f32 fov, f32 zNear, f32 zFar) {
        Camera* camera = new FreeCamera(ctx, fov, zNear, zFar);
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
    /// @brief Create a new point light source
    /// @return Light Light
    ///
    inline Light* createPointLight() {
        if (pointsLights.size() <= numberOfPointLights) {
            spdlog::error("You created more than 16 points lights");
            return nullptr;
        }

        Light* light = new Light();
        pointsLights.at(numberOfPointLights++) = light;
        return light;
    }

    ///
    /// @brief Create a new directional light source
    /// @return Light Light
    ///
    inline Light* createDirectionalLight() {
        if (directionalLights.size() <= numberOfDirectionalLights) {
            spdlog::error("You created more than 8 directional lights");
            return nullptr;
        }

        Light* light = new Light();
        directionalLights.at(numberOfDirectionalLights++) = light;
        return light;
    }

    ///
    /// @brief Render called each frame
    /// @param deltaTime Elapsed time between each frame
    ///
    void render(f32 deltaTime) override {
        mainCamera->update(deltaTime);

        onRender(this, deltaTime);

        // lights
        for (size_t i = 0; i < numberOfDirectionalLights; i++) {
            auto light = directionalLights[i];
            std::stringstream ss;
            ss << "directionalLights[" << i << "]";
            ctx->shaderUniform(ss.str() + ".color", light->color.toVec());
            ctx->shaderUniform(ss.str() + ".position", light->position);
            ctx->shaderUniform(ss.str() + ".intensity", light->intensity);
        }
        ctx->shaderUniform("directionalLightsCount", (i32)numberOfDirectionalLights);

        for (size_t i = 0; i < numberOfPointLights; i++) {
            auto light = pointsLights[i];
            std::stringstream ss;
            ss << "pointLights[" << i << "]";
            ctx->shaderUniform(ss.str() + ".color", light->color.toVec());
            ctx->shaderUniform(ss.str() + ".position", light->position);
            ctx->shaderUniform(ss.str() + ".intensity", light->intensity);
        }
        ctx->shaderUniform("pointLightsCount", (i32)numberOfPointLights);
    }
};
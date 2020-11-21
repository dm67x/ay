#pragma once

#include <string>

const std::string BLINN_PHONG_VERTEX = " \
    #version 320 es\n \
    layout(location = 0) in vec3 position; \
    layout(location = 1) in vec3 normal; \
    layout(location = 2) in vec2 uv; \
    layout(location = 3) in vec3 color; \
    \
    out VS_OUT{ \
        vec3 position; \
        vec3 normal; \
        vec2 uv; \
        vec3 color; \
    } vs_out; \
    \
    uniform mat4 projectionMatrix; \
    uniform mat4 viewMatrix; \
    uniform mat4 modelMatrix; \
    uniform mat4 normalMatrix; \
    \
    void main() { \
        vec4 worldPos = modelMatrix * vec4(position, 1.0); \
        gl_Position = projectionMatrix * viewMatrix * worldPos; \
        vs_out.position = worldPos.xyz / worldPos.w; \
        vs_out.normal = (normalMatrix * vec4(normal, 1.0)).xyz; \
        vs_out.uv = uv; \
        vs_out.color = color; \
    } \
";

const std::string BLINN_PHONG_FRAGMENT = " \
    #version 320 es\n \
    precision mediump float; \
    \
    out vec4 fragOut; \
    \
    in VS_OUT{ \
        vec3 position; \
        vec3 normal; \
        vec2 uv; \
        vec3 color; \
    } fs_in; \
    \
    uniform bool isAxis; \
    uniform vec3 cameraPosition; \
    uniform sampler2D albedo; \
    \
    struct Light { \
        vec3 position; \
        vec4 color; \
        float intensity; \
    }; \
    \
    uniform int pointLightsCount; \
    uniform int directionalLightsCount; \
    uniform Light pointLights[16]; \
    uniform Light directionalLights[8]; \
    \
    struct Material { \
        vec4 baseColor; \
        float metallicFactor; \
        float roughnessFactor; \
        vec4 emissiveFactor; \
    }; \
    \
    uniform Material material; \
    \
    const vec3 ambientColor = vec3(0.1, 0.1, 0.1); \
    const vec3 diffuseColor = vec3(0.5, 0.5, 0.5); \
    const vec3 specColor = vec3(1.0, 1.0, 1.0); \
    const float shininess = 8.0; \
    \
    vec3 computeLight(Light light, bool isPoint) { \
        vec3 N = normalize(fs_in.normal); \
        vec3 L = light.position - fs_in.position; \
        float distance = length(L); \
        distance = distance * distance; \
        L = normalize(L); \
        float specular = 0.0; \
        float lambertian = max(dot(L, N), 0.0); \
        \
        if (lambertian > 0.0) { \
            vec3 V = normalize(cameraPosition - fs_in.position); \
            vec3 H = (L + V) / length(L + V); \
            float angle = max(dot(H, N), 0.0); \
            specular = pow(angle, shininess); \
        } \
        \
        float intensity = light.intensity; \
        if (isPoint) { \
            intensity *= 1.0 / distance; \
        } \
        \
        return ambientColor + \
            material.baseColor.rgb * lambertian * light.color.xyz * intensity + \
            specColor * specular * light.color.xyz * intensity; \
    } \
    \
    void main() { \
        vec3 color = vec3(0, 0, 0); \
        \
        for (int i = 0; i < directionalLightsCount; i++) { \
            color += computeLight(directionalLights[i], false); \
        } \
        \
        for (int i = 0; i < pointLightsCount; i++) { \
            color += computeLight(pointLights[i], true); \
        } \
        \
        if (isAxis) { \
            fragOut = vec4(fs_in.color, 1.0); \
        } \
        else { \
            fragOut = vec4(color, 1.0); \
        } \
    } \
";
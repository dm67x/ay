#pragma once

#include "color.hpp"
#include "context.hpp"
#include <string>

struct Material {
    std::string name;
    Color baseColor;
    Texture2D baseColorTexture;
    float metallicFactor;
    float roughnessFactor;
    Texture2D metallicRoughnessTexture;
    Texture2D normalTexture;
    Texture2D occlusionTexture;
    Texture2D emissiveTexture;
    Color emissiveFactor;
    enum class AlphaMode {
        OPAQUE_MODE,
        MASK_MODE,
        BLEND_MODE
    } alphaMode;
    float alphaCutoff;
    bool doubleSided;

    Material()
        : name(""),
        baseColor(Color::white()),
        baseColorTexture(0),
        metallicFactor(1.f),
        roughnessFactor(1.f),
        metallicRoughnessTexture(0),
        normalTexture(0),
        occlusionTexture(0),
        emissiveTexture(0),
        emissiveFactor(Color::black()),
        alphaMode(AlphaMode::OPAQUE_MODE),
        alphaCutoff(0.5f),
        doubleSided(false)
    {
    }
};
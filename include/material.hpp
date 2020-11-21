#pragma once

#include "types.hpp"
#include "color.hpp"
#include "context.hpp"
#include <string>

class Material {
public:
    ///
    /// @brief Constructor
    ///
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

    /// 
    /// @brief Comparaison operator
    /// @param m Material
    /// 
    inline bool operator==(const Material& m) {
        return name == m.name &&
            baseColor == m.baseColor &&
            baseColorTexture == m.baseColorTexture &&
            metallicFactor == m.metallicFactor &&
            roughnessFactor == m.roughnessFactor &&
            metallicRoughnessTexture == m.metallicRoughnessTexture &&
            normalTexture == m.normalTexture &&
            occlusionTexture == m.occlusionTexture &&
            emissiveTexture == m.emissiveTexture &&
            emissiveFactor == m.emissiveFactor &&
            alphaMode == m.alphaMode &&
            alphaCutoff == m.alphaCutoff &&
            doubleSided == m.doubleSided;
    }

public:
    std::string name;
    Color baseColor;
    Texture2D baseColorTexture;
    f32 metallicFactor;
    f32 roughnessFactor;
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
    f32 alphaCutoff;
    bool doubleSided;
};
#pragma once

#include "Ay.hpp"

#include <vector>
#include <string>

class Material;

class MaterialManager
{
    std::vector<Material> m_materials;

    MaterialManager();
    MaterialManager(const MaterialManager&) = delete;
    void operator=(const MaterialManager&) = delete;

public:
    AY_API static MaterialManager& instance();
    AY_API ~MaterialManager();

    AY_API void add(const Material&);
    AY_API Material* get(const std::string&);
};
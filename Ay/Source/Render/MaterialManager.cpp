#include "Render/MaterialManager.hpp"
#include "Render/Material.hpp"

MaterialManager::MaterialManager()
    : m_materials{}
{
}

MaterialManager& MaterialManager::instance()
{
    static MaterialManager instance;
    return instance;
}

MaterialManager::~MaterialManager()
{
}

void MaterialManager::add(const Material& material)
{
    m_materials.push_back(material);
}

Material* MaterialManager::get(const std::string& name)
{
    for (auto it = m_materials.begin(); it != m_materials.end(); it++) {
        if (it->name() == name) {
            return &(*it);
        }
    }
    return nullptr;
}
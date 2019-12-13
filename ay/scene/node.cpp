#include "node.h"
#include "render/gcard_renderer.h"
#include "render/camera.h"
#include "entity.h"

SceneNode::SceneNode()
    : m_nodes{},
    m_entity{ nullptr },
    m_position{ 0, 0, 0 }
{
}

SceneNode::~SceneNode()
{
    for (auto node : m_nodes) {
        delete node;
    }
}

SceneNode* SceneNode::operator[](int index) const
{
    return m_nodes[index];
}

SceneNode* SceneNode::create()
{
    SceneNode* node = new SceneNode;
    m_nodes.push_back(node);
    return node;
}

void SceneNode::attach(SceneEntity* entity)
{
    m_entity = entity;
}

void SceneNode::position(glm::vec3 position)
{
    m_position = position;
}

void SceneNode::draw() const 
{
    if (m_entity)
        m_entity->draw();

    for (auto node : m_nodes) {
        node->draw();
    }
}
#pragma once

#include <glm/glm.hpp>

struct Vertex
{
    glm::vec3 position = glm::vec3(0);
    glm::vec3 normal = glm::vec3(0);
    glm::vec2 uv = glm::vec2(0);
    glm::vec4 color = glm::vec4(1);
};
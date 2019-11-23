#pragma once

#include <glm/glm.hpp>

struct Vertex
{
    glm::vec3 position = glm::vec3(0);
    glm::vec3 normal = glm::vec3(0);
    glm::vec2 uv = glm::vec2(0);
    glm::vec4 color = glm::vec4(1);

    friend bool operator==(const Vertex& v1, const Vertex& v2)
    {
        if (v1.position != v2.position) return false;
        if (v1.normal != v2.normal) return false;
        if (v1.uv != v2.uv) return false;
        if (v1.color != v2.color) return false;
        return true;
    }
};
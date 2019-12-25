#include "light.hpp"
#include "rendering/shader.hpp"

Light::Light(const std::string& name)
    : Entity(name)
{
}

void Light::draw(const Shader& program, const glm::mat4& tr) const
{
    program.uniform("lightPosition", 
        glm::vec3(tr * glm::vec4(position, 1)));
    program.uniform("lightPower", power);
    program.uniform("lightColor", color);
}
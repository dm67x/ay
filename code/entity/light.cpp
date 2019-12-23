#include "light.hpp"
#include "shader/shader.hpp"

Light::Light(const std::string& name)
    : Entity(name)
{
}

void Light::draw(const Shader& program) const
{
    program.uniform("lightPosition", position);
    program.uniform("lightPower", power);
    program.uniform("lightColor", color);
}
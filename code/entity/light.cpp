#include "light.hpp"
#include "shader/shader.hpp"

void Light::draw(const Shader& program) const
{
    program.uniform("lightPosition", position);
    program.uniform("lightPower", power);
    program.uniform("lightColor", color);
}
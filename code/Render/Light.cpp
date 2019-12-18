#include "Light.hpp"

void Light::draw(const ShaderProgram& program) const
{
    program.uniform("lightPosition", position);
    program.uniform("lightPower", power);
    program.uniform("lightColor", color);
}
#include "light.h"

void Light::draw(const Shader& program) const
{
    program.uniform("lightPosition", position);
    program.uniform("lightPower", power);
    program.uniform("lightColor", color);
}
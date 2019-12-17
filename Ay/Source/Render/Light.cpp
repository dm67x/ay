#include "render/Light.hpp"

void Light::draw(const ShaderProgram& program) const
{
    program.uniform("lightPosition", position);
}
#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec4 color;

out vec4 colorOut;

uniform mat4 MVP;

void main() {
    gl_Position = MVP * vec4(position, 1.0);
    colorOut = vec4(position, 1.0);
}
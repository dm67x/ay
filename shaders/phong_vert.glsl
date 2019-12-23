#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec4 color;

out vec3 positionOut;
out vec4 colorOut;
out vec3 normalOut;
out vec2 uvOut;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main() {
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
    colorOut = color;
    uvOut = uv;
    vec4 pos = modelMatrix * vec4(position, 1.0);
    positionOut = vec3(pos) / pos.w;
    mat4 normalMatrix = transpose(inverse(viewMatrix * modelMatrix));
    normalOut = (normalMatrix * vec4(normal, 0)).xyz;
}
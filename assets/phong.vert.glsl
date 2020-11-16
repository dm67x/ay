#version 320 es

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec3 color;

out VS_OUT {
    vec3 position;
    vec3 normal;
    vec2 uv;
    vec3 color;
} vs_out;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 normalMatrix;

void main() {
    vec4 worldPos = modelMatrix * vec4(position, 1.0);
    gl_Position = projectionMatrix * viewMatrix * worldPos;
    vs_out.position = worldPos.xyz / worldPos.w;
    vs_out.normal = (normalMatrix * vec4(normal, 1.0)).xyz;
    vs_out.uv = uv;
    vs_out.color = color;
}
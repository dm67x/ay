#version 320 es

layout(location = 0) in vec3 positionIn;
layout(location = 1) in vec3 normalIn;
layout(location = 2) in vec2 uvIn;

out vec3 positionOut;
out vec3 normalOut;
out vec2 uvOut;

out VS_OUT {
    vec3 position;
    vec3 normal;
    vec2 uv;
} vs_out;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 normalMatrix;

void main() {
    vec4 worldPos = modelMatrix * vec4(positionIn, 1.0);
    gl_Position = projectionMatrix * viewMatrix * worldPos;
    vs_out.position = worldPos.xyz / worldPos.w;
    vs_out.normal = (normalMatrix * vec4(normalIn, 1.0)).xyz;
    vs_out.uv = uvIn;
}
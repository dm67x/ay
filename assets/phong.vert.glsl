#version 320 es

layout(location = 0) in vec3 positionIn;
layout(location = 1) in vec3 normalIn;
layout(location = 2) in vec2 uvIn;

out vec3 positionOut;
out vec3 normalOut;
out vec2 uvOut;

uniform mat4 projectionViewMatrix;
uniform mat4 modelMatrix;
uniform mat4 normalMatrix;

void main() {
    vec4 worldPos = modelMatrix * vec4(positionIn, 1.0);
    gl_Position = projectionViewMatrix * worldPos;
    positionOut = worldPos.xyz / worldPos.w;
    normalOut = (normalMatrix * vec4(normalIn, 1.0)).xyz;
    uvOut = uvIn;
}
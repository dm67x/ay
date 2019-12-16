#version 330
            
in vec4 colorOut;
in vec3 normalOut;
in vec2 uvOut;

out vec4 fragOut;

void main() {
    fragOut = colorOut * vec4(normalOut, 1.0);
}
#version 330
            
in vec4 colorOut;
out vec4 fragOut;

void main() {
    fragOut = colorOut + 1.0;
}
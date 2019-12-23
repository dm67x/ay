#version 330

precision highp float;
 
in vec3 positionOut;
in vec4 colorOut;
in vec3 normalOut;
in vec2 uvOut;

out vec4 fragOut;

uniform vec3 Ka;
uniform vec3 Ks;
uniform vec3 Kd;
uniform vec3 Ke;
uniform float Ns;
uniform float Ni;
uniform vec3 cameraPosition;
uniform vec3 lightPosition;
uniform vec3 lightColor;
uniform float lightPower;
uniform int mapKdValid;
uniform sampler2D mapKd;

void main() {
    vec3 normal = normalize(normalOut);
    vec3 lightDir = lightPosition - positionOut;
    float distance = length(lightDir);
    distance = distance * distance;
    lightDir = normalize(lightDir);

    float lambertian = max(dot(lightDir, normal), 0);
    float specular = 0;

    if (lambertian > 0) {
        vec3 viewDir = normalize(cameraPosition - positionOut);
        vec3 halfDir = normalize(lightDir + viewDir);
        float specAngle = max(dot(halfDir, normal), 0);
        specular = pow(specAngle, Ns);
    }

    vec4 texKd = vec4(1);
    if (mapKdValid == 1) {
        texKd = texture(mapKd, uvOut);
    }

    vec3 colorLinear =  
        Kd * texKd.rgb * lambertian * lightColor * lightPower / distance +
        Ks * specular * lightColor * lightPower / distance;

    fragOut = vec4(normalOut, 1.0);
}
#version 330
 
in vec3 positionOut;
in vec4 colorOut;
in vec3 normalOut;
in vec2 uvOut;
in mat4 normalMatrix;

out vec4 fragOut;

uniform vec3 Ka;
uniform vec3 Ks;
uniform vec3 Kd;
uniform vec3 Ke;
uniform float Ns;
uniform float Ni;
uniform vec3 cameraPosition;
uniform vec3 lightPosition;

void main() {
    vec3 normal = normalize(normalOut);
    vec3 lightDir = normalize(lightPosition - positionOut);
    vec3 viewDir = normalize(cameraPosition - positionOut);
    vec3 reflectDir = reflect(-lightDir, normal);

    vec3 ambiant = colorOut.xyz * Kd;
    vec3 diffuse = Kd * max(dot(lightDir, normal), 0);
    vec3 specular = Ks * pow(max(dot(reflectDir, viewDir), 0), Ns);

    fragOut = vec4(ambiant + diffuse + specular, 1);
}
#version 320 es
precision mediump float;

out vec4 fragOut;

in VS_OUT {
    vec3 position;
    vec3 normal;
    vec2 uv;
} fs_in;

uniform sampler2D albedo;

struct Light {
    vec3 position;
    vec4 color;
    float power;
};

uniform int lightsCount;
uniform Light lights[16];

const vec3 ambientColor = vec3(0.1, 0.1, 0.1);
const vec3 diffuseColor = vec3(0.5, 0.5, 0.5);
const vec3 specColor = vec3(1.0, 1.0, 1.0);
const float shininess = 16.0;
const float screenGamma = 2.2;

vec3 computePointLight(Light light) {
    vec3 normal = normalize(fs_in.normal);
    vec3 lightDir = light.position - fs_in.position;
    float distance = length(lightDir);
    distance = distance * distance;
    lightDir = normalize(lightDir);

    float lambertian = max(dot(lightDir, normal), 0.0);
    float specular = 0.0;

    if (lambertian > 0.0) {
        vec3 viewDir = normalize(-fs_in.position);
        vec3 halfDir = normalize(lightDir + viewDir);
        float specAngle = max(dot(halfDir, normal), 0.0);
        specular = pow(specAngle, shininess);
    }

    vec3 colorLinear = 
        ambientColor + 
        diffuseColor * lambertian * light.color.xyz * light.power / distance +
        specColor * specular * light.color.xyz * light.power / distance;

    return pow(colorLinear, vec3(1.0 / screenGamma));
}

void main() {
    vec3 color = vec3(0, 0, 0);
    for (int i = 0; i < lightsCount; i++) {
        color += computePointLight(lights[i]);
    }
    fragOut = vec4(color, 1.0);
}
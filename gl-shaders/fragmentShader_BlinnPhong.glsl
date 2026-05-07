#version 330 core

layout(location=0) out vec4 fragmentColor;

uniform vec4 lightIntensity;
uniform vec4 ambientLight;
uniform vec4 diffuseComponent;
uniform vec4 specularComponent;
uniform float phongExponent;
uniform sampler2D textureSampler;

in vec4 normal;
in vec4 lightDir;
in vec4 halfVector;
in vec2 textureUV;

void main(void) {
    vec4 n = normalize(normal);
    vec4 l = normalize(lightDir);
    vec4 h = normalize(halfVector);

    /* Calculate the lambertian part. */
    float lVal = max(0.0, dot(n, l));
    vec3 diffuseShading = vec3(diffuseComponent.r * lVal, diffuseComponent.g * lVal, diffuseComponent.b * lVal);

    /* Calculate the phong part. */
    float phongVal = pow(max(0.0, dot(n, h)), phongExponent);
    vec3 specularShading = vec3(specularComponent.r * phongVal, specularComponent.g * phongVal, specularComponent.b * phongVal);

    //fragmentColor = vec4(diffuseShading + specularShading, 1.0);

    fragmentColor = ambientLight + lightIntensity * texture(textureSampler, textureUV) * vec4(diffuseShading + specularShading, 1.0);
}

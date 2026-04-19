#version 330 core

layout(location=0) out vec4 fragmentColor;

uniform vec4 diffuseComponent;
uniform vec4 specularComponent;
uniform float phongExponent;

vec4 in normal;
vec4 in lightDir;
vec4 in halfVector;

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

    fragmentColor = vec4(0.5, 0.5, 0.5, 1.0);
}
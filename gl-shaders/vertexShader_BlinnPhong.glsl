#version 330 core

layout(location=0) in vec3 in_Position;
layout(location=1) in vec3 in_Normal;
layout(location=2) in vec2 in_TextureUV;

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 normalMatrix;

uniform vec4 lightPosWorld;
uniform vec4 eyePosWorld;

out vec4 normal;
out vec4 lightDir;
out vec4 halfVector;
out vec2 textureUV;

void main(void) {
    normal = normalize(normalMatrix * vec4(in_Normal, 0.0));

    vec4 vertexWorldPos = modelMatrix * vec4(in_Position, 1.0);
    vec4 eyeDir = normalize(eyePosWorld - vertexWorldPos);
    halfVector = normalize(eyeDir + lightDir);

    lightDir = eyeDir;


    gl_Position = projMatrix * viewMatrix * vertexWorldPos;

    textureUV = in_TextureUV;
}
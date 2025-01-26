#version 330

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec2 aVertexTexture;

out vec3 vFragColor;

uniform float uTime = 45;
uniform mat3 uModelMatrix;

void main() {
    vFragColor = vec3(1, 0, 0);
    vec2 eVertexPosition = (uModelMatrix * vec3(aVertexPosition, 1)).xy;
    gl_Position = vec4(eVertexPosition, 0, 1);
}
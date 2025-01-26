#version 330

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec2 aVertexTexture;

out vec3 vFragColor;

uniform mat3 uModelMatrix;
uniform vec3 uColor;

void main() {
    vFragColor = uColor;
    vec2 eVertexPosition = (uModelMatrix * vec3(aVertexPosition, 1)).xy;
    gl_Position = vec4(eVertexPosition, 0, 1);
}
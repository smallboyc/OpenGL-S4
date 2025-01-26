#version 330

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec2 aVertexTexture;

out vec2 texCoords;

void main() {
    texCoords = aVertexTexture;
    gl_Position = vec4(aVertexPosition, 0, 1);
}
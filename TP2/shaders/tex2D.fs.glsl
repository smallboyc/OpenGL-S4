#version 330

in vec2 texCoords;

out vec4 fFragTex;

uniform sampler2D uTexture;

void main() {
    fFragTex = texture(uTexture, texCoords);
}
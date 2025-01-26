#version 330

in vec3 vFragColor;

out vec3 fFragColor;

void main() {
    float M = (vFragColor.r + vFragColor.g + vFragColor.b) / 3.0;
    fFragColor = vec3(M, M, M);
}
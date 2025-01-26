#version 330

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec3 aVertexColor;

out vec3 vFragColor;

mat3 translate(float tx, float ty) {
    return mat3(vec3(1, 0, 0), vec3(0, 1, 0), vec3(tx, ty, 1));
}

mat3 scale(float sx, float sy) {
    return mat3(vec3(sx, 0, 0), vec3(0, sy, 0), vec3(0, 0, 1));
}

mat3 rotate(float angle_deg) {
    float angle_rad = radians(angle_deg);
    return mat3(vec3(cos(angle_rad), sin(angle_rad), 0), vec3(-sin(angle_rad), cos(angle_rad), 0), vec3(0, 0, 1));
}

void main() {
    vFragColor = aVertexColor;
    // vec2 eVertexPosition = (translate(0.5, 0.5) * vec3(aVertexPosition, 1)).xy;
    //scale, then translate, then rotate, then translate :
    vec2 eVertexPosition = (scale(0.25, 0.25) * translate(2, 0.0) * rotate(45) * translate(0, 3) * vec3(aVertexPosition, 1)).xy;
    gl_Position = vec4(eVertexPosition, 0, 1);
}
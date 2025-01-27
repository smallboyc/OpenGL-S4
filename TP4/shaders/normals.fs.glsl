#version 330

//intput from vs
in vec3 vPosition_vs;
in vec3 vNormal_vs;
in vec2 vTexCoords;

out vec4 fFragTex;

void main() {
    fFragTex = vec4(normalize(vNormal_vs), 0);
}
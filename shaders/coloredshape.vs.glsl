#version 430 core

layout(location = 0) in vec2 vPos;

uniform mat4 uProjection;


out vec2 fInternalPos;

void main() {
    gl_Position = uProjection * vec4(vPos, 0.0, 1.0);

    fInternalPos = vPos;
}
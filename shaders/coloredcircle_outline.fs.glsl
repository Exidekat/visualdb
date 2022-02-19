#version 430 core

layout(location = 0) in vec2 vPos;

uniform mat4 uProjection;
uniform float uOuterRadius2; // r^2
uniform float uInnerRadius2; // r^2
uniform vec2 uCenter;

in vec2 fInternalPos;

out vec4 colorOut;


float len2(vec2 a) {
    return a.x * a.x + a.y * a.y;
}

float dist2(vec2 a, vec2 b) {
    return len2(b - a);
}


void main() {
    if (dist2(fInternalPos, uCenter) > uOuterRadius2 || dist2(fInternalPos, uCenter) < uInnerRadius2) {
        discard;
    } else {
        colorOut = uColor;
    }
}

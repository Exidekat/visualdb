#version 430 core

uniform float uRadius2; // r^2
uniform vec2 uCenter;
uniform vec4 uColor;

in vec2 fInternalPos;

out vec4 colorOut;

float len2(vec2 a) {
    return a.x * a.x + a.y * a.y;
}

float dist2(vec2 a, vec2 b) {
    return len2(b - a);
}

void main() {
    if (dist2(fInternalPos, uCenter) > uRadius2) {
        discard;
    } else {
        colorOut = uColor;
    }
}

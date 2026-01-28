#version 430 core

in vec4 vColor;
in vec2 vPosition;

out vec4 fragColor;

void main() {
    vec2 c = vPosition * 2.0 - 1.0;
    if(dot(c,c) > 1.0) discard;
    float a = exp(-dot(c,c) * 2.0) * vColor.a ;
    if(a < 0.05) discard;
    fragColor = vec4(vColor.rgb , a);
}
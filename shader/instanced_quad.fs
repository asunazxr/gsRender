#version 430 core

in vec4 vColor;
in vec2 vPosition;
in mat2 conic;
out vec4 fragColor;

void main () {
    float power = -0.5 * dot(vPosition,conic * vPosition);
    if(power > 0.0) discard;
    float a = vColor.a * exp(power);
    if(a < 0.004) discard;
    fragColor = vec4(vColor.rgb * a, a);
    //fragColor = vec4(1.0,0.0,0.0,1.0);
}
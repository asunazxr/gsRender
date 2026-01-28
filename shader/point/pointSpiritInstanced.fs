#version 430 core
in vec4 vColor;
in vec2 vScreenPos;
in mat2 conic;
out vec4 fragColor;



void main() {
    // 当前片段位置
    vec2 fragCoord = gl_FragCoord.xy;
    // 到中心的偏移
    vec2 delta = fragCoord - vScreenPos;
    float c = dot(delta,conic * delta);
    if(c>1) discard;
    float power = -0.5 * c;
    float a = vColor.a * exp(power);
    fragColor = vec4(vColor.rgb , a);
}
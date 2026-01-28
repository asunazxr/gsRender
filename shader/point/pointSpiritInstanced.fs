#version 430 core
in vec4 vColor;
in vec2 vScreenPos;
in float vPointSize;
in mat2 conic;

out vec4 fragColor;


// 2D高斯函数
float gaussian2D(vec2 x, mat2 sigmaInv) {
    float exponent = -0.5 * dot(x, sigmaInv * x);
    return exp(exponent);
}

void main() {
    // 当前片段位置
    vec2 fragCoord = gl_FragCoord.xy;
    // 到中心的偏移
    vec2 delta = fragCoord - vScreenPos;
    // 简单的圆形高斯
    float dist2 = dot(delta, delta);
    float radius = vPointSize * 0.5;
    //超出半径就丢弃
    if (dist2 > radius * radius) {
        discard;
    }
    float a = vColor.a * gaussian2D(delta,conic);
    fragColor = vec4(vColor.rgb * a, a);
}
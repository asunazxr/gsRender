#version 430 core
layout(location=0) in vec2 aQuadPos;


layout(location=1) in vec3 aInstancePos;
layout(location=2) in vec4 aInstanceColor;
layout(location=3) in vec3 cov3d_upper;  // a, b, c
layout(location=4) in vec3 cov3d_lower;  // d, e, f

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec2 viewportSize;

out vec2 vPosition;
out vec4 vColor;
out mat2 conic;

void main() {
    //相机视角下示例中心点坐标
    vec4 cam =  view * model * vec4(aInstancePos,1.0);
    //裁剪空间坐标
    vec4 pos2d = projection * cam;
    pos2d.xyz = pos2d.xyz / pos2d.w;
    //视锥剔除
    if(any(greaterThan(abs(pos2d.xyz),vec3(1.3)))){
        gl_Position = vec4(-100,-100,-100,-1);
        return;
    }
    //获取三维协方差
    mat3 cov3d = mat3(
        cov3d_upper.x, cov3d_upper.y, cov3d_upper.z,
        cov3d_upper.y, cov3d_lower.x, cov3d_lower.y,
        cov3d_upper.z, cov3d_lower.y, cov3d_lower.z
    );
    //计算三维雅可比矩阵
    float z = cam.z;
    float fx = projection[0][0] * viewportSize.x * 0.5;
    float fy= projection[1][1] * viewportSize.y * 0.5;
    mat3 J =  mat3(
        fx / z,   0.0, -fx * cam.x / (z * z),
        0,     fy / z, -fy * cam.y / (z * z),
        0,     0,       0
    );
    //计算二维协方差
    mat3 W = mat3(view);
    mat3 T = J*W;
    mat3 cov2d_3 = T * cov3d * transpose(T);
    mat2 cov2d = mat2(cov2d_3);
    
    float det = cov2d[0][0] * cov2d[1][1] - cov2d[0][1] * cov2d[1][0];
    //二维协方差求逆，用于计算权重
    conic = inverse(cov2d);


    //将四边形中心移动到高斯点，此时高斯点在裁剪空间
    //根据协方差计算四边形大小
    vec2 qualSize = 3.0 * vec2(sqrt(cov2d[0][0]),sqrt(cov2d[1][1]));
    //高斯点在屏幕坐标
    vec2 guassCenter = (vec2(pos2d) + 1.0) * 0.5 *viewportSize;


    vec2 vertexPos = guassCenter + aQuadPos * qualSize;
    gl_Position = vec4((vertexPos / viewportSize) * 2.0 - 1.0, pos2d.z, 1.0);

    vColor = aInstanceColor;
    vPosition = aQuadPos  ;
}
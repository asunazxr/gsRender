#version 430 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aInstancePos;
layout(location=2) in vec4 aInstanceColor;
layout(location=3) in vec3 cov3d_upper;
layout(location=4) in vec3 cov3d_lower;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec2 viewportSize;

out vec4 vColor;
out vec2 vScreenPos;
out mat2 conic;

void main() {
    // 视图空间位置
    vec4 cam = view * model * vec4(aInstancePos + aPos, 1.0);
    vec4 clipPos = projection * cam;
    gl_Position = clipPos;
    
    vColor = aInstanceColor;
    // 屏幕空间位置
    vec3 ndc = clipPos.xyz / clipPos.w;
    vScreenPos = (ndc.xy + 1.0) * 0.5 * viewportSize;

    mat3 cov3d = mat3(
        cov3d_upper.x, cov3d_upper.y, cov3d_upper.z,
        cov3d_upper.y, cov3d_lower.x, cov3d_lower.y,
        cov3d_upper.z, cov3d_lower.y, cov3d_lower.z
    );
    
    vec2 focal = vec2(projection[0][0] * viewportSize.x * 0.5, projection[1][1] * viewportSize.y * 0.5);
    mat3 J = mat3(
        focal.x / cam.z, 0., -(focal.x * cam.x) / (cam.z * cam.z),
        0., -focal.y / cam.z, (focal.y * cam.y) / (cam.z * cam.z),
        0., 0., 0.
    );
    mat3 T = transpose(mat3(view) * J);
    mat3 cov2d_3 = transpose(T) * cov3d * T;
    conic = inverse(mat2(cov2d_3));
    vec2 pointSize = 3.0 * vec2(sqrt(cov2d_3[0][0]), sqrt(cov2d_3[1][1]));

    gl_PointSize = max(pointSize.x,pointSize.y);
}

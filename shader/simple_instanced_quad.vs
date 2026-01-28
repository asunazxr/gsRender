#version 430 core
layout(location=0) in vec2 aQuadPos;


layout(location=1) in vec3 aInstancePos;
layout(location=2) in vec3 aInstanceScales;
layout(location=3) in vec4 aInstanceRot;
layout(location=4) in vec4 aInstanceColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 vPosition;
out vec4 vColor;

mat3 getRotationMatrix(vec4 q) {
    float x=q.x, y=q.y, z=q.z, w=q.w;
    float x2=x+x, y2=y+y, z2=z+z;
    float xx=x*x2, xy=x*y2, xz=x*z2, yy=y*y2, yz=y*z2, zz=z*z2;
    float wx=w*x2, wy=w*y2, wz=w*z2;
    return mat3(1.0-(yy+zz), xy-wz, xz+wy, xy+wz, 1.0-(xx+zz), yz-wx, xz-wy, yz+wx, 1.0-(xx+yy));
}

void main() {
    vec3 pos = aInstanceScales * vec3(aQuadPos,1.0);
    pos = getRotationMatrix(aInstanceRot) * pos;
    gl_Position = projection * view * model * vec4(pos + aInstancePos, 1.0);

    vColor = aInstanceColor;
    vPosition = aQuadPos;
}
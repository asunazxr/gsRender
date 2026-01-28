#pragma once
#include "glad/glad.h"
#include<happly-master/happly.h>
#include<glm/glm/glm.hpp>
#include<glm/glm/gtc/quaternion.hpp>
#include<shader.hpp>
#include<widget.hpp>
struct splatPoint{ 
    glm::vec3 position; 
    glm::vec3 scale;
    glm::vec4 rot;
    glm::vec4 color;
    // 协方差矩阵的上三角部分（对称矩阵）
    // cov3d_upper = (cov[0][0], cov[0][1], cov[0][2])
    // cov3d_lower = (cov[1][1], cov[1][2], cov[2][2])
    glm::vec3 cov3d_upper;  // a, b, c
    glm::vec3 cov3d_lower;  // d, e, f
};

enum renderModths{
    POINT_SPIRIT,
    POINT_SPIRIT_INSTANCED,
    INSTANCED_QUAD,
    SIMPLE_INSTANCED_QUAD
};

class gs
{
private:;
	std::string propertyName[14] = { "x","y","z",
									 "opacity",
									 "scale_0","scale_1","scale_2",
									 "rot_0","rot_1","rot_2","rot_3",
	                                 "f_dc_0","f_dc_1","f_dc_2"};
	
    float quadVerts[8] = {
            -1.0f, -1.0f, // 左下
             1.0f, -1.0f, // 右下
             1.0f,  1.0f, // 右上
            -1.0f,  1.0f, // 左上
    };
    float pointVerts[3]={0.0f,0.0f,0.0f};
    const float SH_C0 = 0.28209479177387814;
    
    int num;
    std::string Path;
    GLuint vao, vbo, instVbo;           // OpenGL缓冲区对象
    void init();
    void computecov3d(const glm::vec3& scale, const glm::vec4 rot, glm::vec3& upper, glm::vec3& lower);
public:
    gs( std::string Path);
    bool load();
    void Draw(Shader* shader);
    std::vector<splatPoint> gaussian;
    renderModths defaultModth;
};
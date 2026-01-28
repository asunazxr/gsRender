#pragma once
#include "glm/glm/glm.hpp"
#include"glm/glm/gtc/matrix_transform.hpp"
#include"glm/glm/gtc/quaternion.hpp"
#include "glm/glm/gtc/constants.hpp"
#include "glm/glm/gtx/norm.hpp"
//模型变换组件
struct TransformComponent
{
    glm::vec3 position{0.0f,0.0f,0.0f};
    glm::vec3 rotation{0.0f,0.0f,0.0f};
    glm::vec3 scale{1.0f,1.0f,1.0f};

    TransformComponent(const glm::vec3& p,const glm::vec3& r,const glm::vec3& s){
        position = p;
        rotation = r;
        scale = s;
    }
    /* data */
    // glm::mat4 getModelMatrix(){
    //     glm::mat4 model = glm::mat4(1.0f);
    //     model = glm::translate(model,position);
    //     model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    //     model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    //     model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
    //     model = glm::scale(model,scale);
    //     return model;
    // }
    glm::mat4 getModelMatrix() {
        glm::mat4 model = glm::mat4(1.0f);
        
        // 1. 先缩放（避免影响旋转中心）
        model = glm::scale(model, scale);
        
        // 2. 再旋转（推荐使用四元数）
        // 方法1：四元数旋转（抗万向节锁）
        glm::quat q = glm::quat(glm::radians(rotation));
        model *= glm::toMat4(q);
        
        
        // 3. 最后平移
        model = glm::translate(model, position);
        
        return model;
    }

    void transform(const glm::vec3& transform){ position += transform; }
    void rotate(float degrees,const glm::vec3 axis){ rotation += degrees * axis; }
    void scaling(const glm::vec3& scaling){ scale *= scaling; }
};
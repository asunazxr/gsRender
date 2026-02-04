#pragma once
#include <memory>
#include<3dgs.hpp>
#include<dataset.hpp>
#include<string>

struct ModelComponent{
    bool visible = true;
    gs g;
    std::string modelPath;
    ModelComponent(const std::string& p,const std::shared_ptr<Dataset::Data>& data,bool v)
        :modelPath(p),g(data),visible(v){}
};


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
    glm::mat4 getModelMatrix() {
        glm::mat4 model = glm::mat4(1.0f);
        
        // 1. 先缩放（避免影响旋转中心）
        model = glm::scale(model, scale);
        
        // 2. 再旋转（推荐使用四元数）
        // 方法1：四元数旋转（抗万向节锁）
        glm::quat q = glm::quat(glm::radians(rotation));
        model *= glm::mat4_cast(q);
        
        
        // 3. 最后平移
        model = glm::translate(model, position);
        
        return model;
    }

    void transform(const glm::vec3& transform){ position += transform; }
    void rotate(float degrees,const glm::vec3 axis){ rotation += degrees * axis; }
    void scaling(const glm::vec3& scaling){ scale *= scaling; }
};
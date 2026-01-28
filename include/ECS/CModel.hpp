#pragma once

#include <memory>
#include<3dgs_quad.hpp>
#include<string>
struct ModelComponent{
    bool visible = true;
    std::shared_ptr<gs> model;
    std::string modelPath;
    ModelComponent(const std::string& p,const std::shared_ptr<gs>& m,bool v){
        modelPath = p;
        model = m;
        visible = v;
    }
};
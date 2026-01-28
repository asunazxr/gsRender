#pragma once
#include"ECS/ECSRegistry.hpp"
#include"shader.hpp"
#include"camera.hpp"
#include<vector>
#include<map>
#include<widget.hpp>
class RenderSystem{

public:
    RenderSystem();
    ~RenderSystem();
    void render(widget& widget);
private:
    Shader* shader;
};
#pragma once
#include<shader.hpp>
#include<widget.hpp>
class RenderSystem{

public:
    RenderSystem();
    ~RenderSystem();
    void render(widget& widget);
private:
    Shader* shader;
};
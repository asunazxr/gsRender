#pragma once
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "widget.hpp"

struct myImGuiData{
    bool showModelImportPopup = false;
    bool showMainMenuBar = true;
    bool showCameraSetting = false;

};


class myImGui{
public:
    myImGui(GLFWwindow* window,const char* glsl_version);
    ~myImGui() = default;

    void render(widget& widget);
private:
    void init(GLFWwindow* window,const char* glsl_version);
    widget* w;
    myImGuiData data;
    char modelPath[256] = "";    // 存储模型路径 
};
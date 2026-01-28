#pragma once
#include<glad/glad.h>
#include<glfw/glfw3.h>
#include<glm/glm/glm.hpp>
#include<camera.hpp>
#include<ECS/ECSRegistry.hpp>
class widget
{
private:
    GLFWwindow* window;
    const char* glsl_version = "#version 430";
    
    const char* title = "minRender";
    double lastMouseX = 0.0;
    double lastMouseY = 0.0;
    bool firstMouse = true;
   
    void initGlfwWindow();
    void processKey(GLFWwindow* window,float deltaTime);
    void processMouse(GLFWwindow* window);
public:
    widget(int w,int h,glm::vec4 bgcolor);
    void show();   
    glm::vec4 bgcolor;
    Camera camera;
    int w,h;
    std::vector<entt::entity> entities;
    void addEntity(const std::string& path);
    void deleteEnitity(entt::entity entity);
};
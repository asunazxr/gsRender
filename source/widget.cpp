#include<widget.hpp>
#include<iostream>
#include<mainMenu.hpp>
#include<EntitylFactory.hpp>
#include<ECS/ECSRegistry.hpp>
#include<ECS/RenderSystem.hpp>
#include<imgui/imgui.h>
static void error_callback(int error, const char* description);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

widget::widget(int w ,int h ,glm::vec4 bgcolor){
    this->w = w;
    this->h = h;
    this->bgcolor = bgcolor;
    this->camera = Camera();
    glfwSetErrorCallback(error_callback);
    initGlfwWindow();
}

void widget::initGlfwWindow(){
    
    if(!glfwInit()){
        glfwTerminate();
        return;
    }
    //指明 OpenGL 配置
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    this->window = glfwCreateWindow(this->w, this->h, this->title, NULL, NULL);
   
    if(!this->window){
        glfwTerminate();
        return;
    }
    glfwSetKeyCallback(this->window, key_callback);
    glfwSetFramebufferSizeCallback(this->window, framebuffer_size_callback);
    glfwSetCursorPosCallback(this->window, cursor_position_callback);
    glfwMakeContextCurrent(this->window);
    glfwSwapInterval(1);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout<<"Failed to initialize GLAD"<<std::endl;
        glfwTerminate();
        return;
    }

} 

void widget::show(){
    myImGui ui(window,glsl_version);
    
    //C:\Users\ASUS\Desktop\proj\minRender\resource\3dgs\shuimiao.ply E:\项目\VS_Proj\readPly\readPly\resource\point_cloud.ply
    RenderSystem rs;
    float curFrame = 0.0f;
    float lastFrame = (float)glfwGetTime();
    float deltaTime = 0.0f;
    
    float scroll = 0.0f;
    while(!glfwWindowShouldClose(this->window)){

        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        glClearColor(this->bgcolor.x,this->bgcolor.y,this->bgcolor.z,this->bgcolor.w);
        
        curFrame = (float)glfwGetTime();
        deltaTime = curFrame - lastFrame;
        lastFrame = curFrame;
        this->processKey(this->window,deltaTime);
        this->processMouse(this->window);
        glfwGetWindowSize (window, &this->w, &this->h);
        //camera.steFocal(this->w);
        camera.setPerspective(this->w,this->h);
        //g.render(*this);
        rs.render(*this);
        ui.render(*this);
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
}

void widget::addEntity(const std::string& path){
    entt::entity entity = EntityFactory::createEntityWithModel(path);
    entities.push_back(entity);

}
void widget::deleteEnitity(entt::entity entity){
    ECSRegistry::getInstance().destory(entity);
    for(auto it = entities.begin();it!=entities.end();){
        if(*it == entity){
            entities.erase(it);
            break;
        }
    }
}

void widget::processKey(GLFWwindow* window,float deltaTime){
    if(glfwGetKey(window,GLFW_KEY_W) == GLFW_PRESS){
        this->camera.ProcessKeyBoard(FORWARD,deltaTime);
    }
    if(glfwGetKey(window,GLFW_KEY_S) == GLFW_PRESS){
        this->camera.ProcessKeyBoard(BACKWARD,deltaTime);
    }
    if(glfwGetKey(window,GLFW_KEY_A) == GLFW_PRESS){
        this->camera.ProcessKeyBoard(LEFT,deltaTime);
    }
    if(glfwGetKey(window,GLFW_KEY_D) == GLFW_PRESS){
        this->camera.ProcessKeyBoard(RIGHT,deltaTime);
    }
}

void widget::processMouse(GLFWwindow* window){
    // Skip camera movement if ImGui wants to capture mouse
    if (ImGui::GetIO().WantCaptureMouse) {
        return;
    }
    
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    
    if (firstMouse) {
        lastMouseX = xpos;
        lastMouseY = ypos;
        firstMouse = false;
        return;
    }
    
    if(glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
        double xoffset = xpos - lastMouseX;
        double yoffset = lastMouseY - ypos;  // reversed: y increases downward in screen space
        this->camera.ProcessMouseMovement(xoffset, yoffset);
    }
    
    lastMouseX = xpos;
    lastMouseY = ypos;
    
}

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}
static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    //std::cout<<"xpos:"<<xpos<<" ypos:"<<ypos<<std::endl;
}

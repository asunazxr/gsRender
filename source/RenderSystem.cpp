#include<ECS/RenderSystem.hpp>
#include<ECS/ECSRegistry.hpp>
#include<ECS/component.hpp>
#include<camera.hpp>

RenderSystem::RenderSystem(){


    std::string vertexPath = "C:\\Users\\ASUS\\Desktop\\proj\\gsViewer\\shader\\instanced_quad.vs";
    std::string fragmentPath = "C:\\Users\\ASUS\\Desktop\\proj\\gsViewer\\shader\\instanced_quad.fs";
    this->shader = new Shader(vertexPath.c_str(),fragmentPath.c_str());
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    // glBlendFuncSeparate(GL_ONE_MINUS_DST_ALPHA,GL_ONE,GL_ONE_MINUS_DST_ALPHA,GL_ONE);
    // glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glBlendFuncSeparate(GL_ONE, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
}
RenderSystem::~RenderSystem(){
    if(shader){
        delete shader;
    }
}



void RenderSystem::render(widget& widget){
    auto& registry = ECSRegistry::getInstance();
    for(auto entity : widget.entities){
        auto& transform = registry.get<TransformComponent>(entity);
        auto& modelComp = registry.get<ModelComponent>(entity);
        this->shader->use();
        this->shader->setVec2("viewportSize",glm::vec2(widget.w,widget.h));
        this->shader->setMat4("model", transform.getModelMatrix());
        this->shader->setMat4("view", widget.camera.getViewMatrix());
        this->shader->setMat4("projection", widget.camera.getPerspective());
        modelComp.g.Draw(shader);
    }


}
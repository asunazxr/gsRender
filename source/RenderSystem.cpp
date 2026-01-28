#include"ECS/RenderSystem.hpp"
#include"ECS/CModel.hpp"
#include"ECS/CTransform.hpp"
#include"shader.hpp"
#include"EntitylFactory.hpp"


RenderSystem::RenderSystem(){
    // std::string vertexPath = "C:\\Users\\ASUS\\Desktop\\proj\\minRender\\shader\\vertex_instanced_quad.shader";
    // std::string fregmentPath = "C:\\Users\\ASUS\\Desktop\\proj\\minRender\\shader\\fragment_instanced_quad.shader";
    // std::string vertexPath = "C:\\Users\\ASUS\\Desktop\\proj\\minRender\\shader\\1.shader";
    // std::string fregmentPath = "C:\\Users\\ASUS\\Desktop\\proj\\minRender\\shader\\2.shader";
    // std::string vertexPath = "C:\\Users\\ASUS\\Desktop\\proj\\minRender\\shader\\point\\pointSpirit.vs";
    // std::string fragmentPath = "C:\\Users\\ASUS\\Desktop\\proj\\minRender\\shader\\point\\pointSpirit.fs";
    std::string vertexPath = "C:\\Users\\ASUS\\Desktop\\proj\\minRender\\shader\\point\\pointSpiritInstanced.vs";
    std::string fragmentPath = "C:\\Users\\ASUS\\Desktop\\proj\\minRender\\shader\\point\\pointSpiritInstanced.fs";
    this->shader = new Shader(vertexPath.c_str(),fragmentPath.c_str());
    glEnable(GL_DEPTH_TEST);
    //glDepthMask(GL_FALSE);
    //glDisable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

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
        this->shader->setMat4("model", transform.getModelMatrix());
        this->shader->setMat4("view", widget.camera.getViewMatrix());
        this->shader->setMat4("projection", widget.camera.getPerspective());
        this->shader->setVec2("viewportSize",glm::vec2(widget.w,widget.h));
        //this->shader->setVec2("focal",widget.camera.focal);
        modelComp.model->Draw(shader);
    }


}
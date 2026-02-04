#include<mainMenu.hpp>
#include<string>
#include<ECS/component.hpp>
myImGui::myImGui(GLFWwindow* window,const char* glsl_version){
    init(window,glsl_version);
}

void myImGui::init(GLFWwindow* window,const char* glsl_version){
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL( window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init(glsl_version);

}

void myImGui::render(widget& widget){
    w = &widget;
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    //场景主窗口
    ImGui::Begin(" ");
    if(ImGui::TreeNode("world setting")){
        //set clear_color
        ImGui::ColorEdit4("clear color",(float*)&widget.bgcolor);
        //model import
        ImGui::Text("Select a model file:");  
        ImGui::InputTextWithHint("##ModelPath", "Enter file path ...", modelPath, IM_ARRAYSIZE(modelPath));   
        ImGui::SameLine();
        if (ImGui::Button("Import")) {  
            if (w && strlen(modelPath) > 0) {  
                // 调用scene的加载模型方法  
                w->addEntity(modelPath); 
            }    
        }
        ImGui::Separator();
        std::string countText = "Model counts: " + std::to_string(w->entities.size());  
        ImGui::Text("%s", countText.c_str()); 
        ImGui::Separator();
        if(ImGui::Button("camera")){
            data.showCameraSetting = true;
        }
        ImGui::TreePop();
    }
    // if(ImGui::TreeNode("camera choice")){
    //     for(int i=0;i<w->entities.size();++i){
    //         std::string model = "Model"+std::to_string(i);
    //         auto& modelComp = ECSRegistry::getInstance().get<ModelComponent>(w->entities[i]);
    //         if(ImGui::Button(model.c_str())){
    //             w->camera = modelComp.model->getGsCamera();

    //         }
            
    //     }
    //     ImGui::TreePop();
    // }
    if(ImGui::TreeNode("scene models")){
        for(int i=0;i<w->entities.size();++i){
            std::string model = "Model"+std::to_string(i);
            auto& translateComp = ECSRegistry::getInstance().get<TransformComponent>(w->entities[i]);
            if(ImGui::TreeNode(model.c_str())){
                ImGui::Separator();
                ImGui::Text("Translate:");
                ImGui::DragFloat3("position",(float*)&translateComp.position,0.01);
                ImGui::DragFloat3("rotation",(float*)&translateComp.rotation,0.1f,-360.0f,360.0f);
                ImGui::DragFloat3("size",(float*)&translateComp.scale,0.1);
                ImGui::Separator();
                ImGui::TreePop();
            }  
        }
        ImGui::TreePop();
    }


    ImGui::End();
    if(data.showCameraSetting){
        ImGui::Begin("Camera setting");
        ImGui::Separator();
        ImGui::DragFloat3("position",(float*)&widget.camera.Position,0.01);
        ImGui::DragFloat("Zoom",(float*)&widget.camera.Zoom,1.0f,1.0f,89.0f);
        ImGui::DragFloat("MovementSpeed",(float*)&widget.camera.MovementSpeed,0.1f,0.1f,10.0f);
        ImGui::DragFloat("MouseSentitivity",(float*)&widget.camera.MouseSentitivity,0.01f,0.01f,0.2f);
        ImGui::DragFloat("near",(float*)&widget.camera.near,0.01f,0.01f,200.0f);
        ImGui::DragFloat("far",(float*)&widget.camera.far,0.01f,1.0f,200.0f);
        ImGui::Separator();
        if(ImGui::Button("close")){
            data.showCameraSetting = false;
        }
        ImGui::End();
    }
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

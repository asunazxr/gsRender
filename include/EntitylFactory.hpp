#pragma once
#include "entt/entt.hpp"
#include "glm/glm/glm.hpp"
#include "ECS/CModel.hpp"
#include "ECS/CTransform.hpp"
#include "ECS/ECSRegistry.hpp"
#include "modelLoader.hpp"

class EntityFactory{

public:
   static entt::entity createEntityWithModel(const std::string& path,
                                             const glm::vec3& position = glm::vec3(0.0f),
                                             const glm::vec3& rotation = glm::vec3(0.0f,0.0,180.0f),  
                                             const glm::vec3& scale = glm::vec3(1.0f)){
      auto& registry = ECSRegistry::getInstance();
      auto model = ModelLoader::getInstance().loadModel(path);

      entt::entity entity = registry.create();
      if(!model){
         registry.destory(entity);
         return entt::null;
      }
      registry.emplace<TransformComponent>(entity, 
         TransformComponent{position, rotation, scale});
      registry.emplace<ModelComponent>(entity,
         ModelComponent{path,model,true});
      return entity;
   }

};
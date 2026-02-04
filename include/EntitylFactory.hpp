#pragma once
#include <entt.hpp>

#include <modelLoader.hpp>

class EntityFactory{

public:
   static entt::entity createEntityWithModel(const std::string& path,
                                             const glm::vec3& position = glm::vec3(0.0f),
                                             const glm::vec3& rotation = glm::vec3(0.0f,180.0f,180.0f),  
                                             const glm::vec3& scale = glm::vec3(1.0f)){
      auto& registry = ECSRegistry::getInstance();
      auto data = ModelLoader::getInstance().loadModel(path);

      entt::entity entity = registry.create();
      if(!data){
         registry.destory(entity);
         return entt::null;
      }
      registry.emplace<TransformComponent>(entity, 
         TransformComponent{position, rotation, scale});
      registry.emplace<ModelComponent>(entity,
         ModelComponent{path,data,true});
      return entity;
   }

};
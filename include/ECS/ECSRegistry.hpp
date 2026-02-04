#pragma once
#include<entt.hpp>

class ECSRegistry
{
public:
    static ECSRegistry& getInstance(){
        static ECSRegistry instance;
        return instance;
     }
     ECSRegistry(const ECSRegistry& other) = delete;
     ECSRegistry& operator = (const ECSRegistry& other)=delete;
     entt:: entity create(){ return mRegistry.create(); }
     void destory(entt::entity entity){ mRegistry.destroy(entity); } 
     template<typename... Components>
     auto view(){ return mRegistry.view<Components...>(); }
     template<typename Component,typename...Args>
     Component& emplace(entt::entity entity,Args&&... args){
        return mRegistry.emplace<Component>(entity, std::forward<Args>(args)...);
     }
     template<typename Component>
     Component& get(entt::entity entity){
        return mRegistry.get<Component>(entity);
     }
     template<typename Component>
     bool has(entt::entity entity){
        return mRegistry.all_of<Component>(entity);
     }

     void clear(){};
     entt::registry& getRegistry(){ return mRegistry; }

private:
    ECSRegistry(/* args */){};
    ~ECSRegistry() = default;
    entt::registry mRegistry;
};




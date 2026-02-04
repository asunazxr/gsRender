//加载模型
#pragma once

#include<memory>
#include<unordered_map>
#include<mutex>
#include<iostream>

#include<ECS/component.hpp>
#include<future>
#include<dataset.hpp>

class ModelLoader
{
public: 

    ModelLoader(const ModelLoader& other)=delete;
    ModelLoader& operator=(const ModelLoader& other)=delete;

    static ModelLoader& getInstance(){
        static ModelLoader instance;
        return instance;
    }
    

    std::shared_ptr<Dataset::Data> loadModel(const std::string& filePath);
    // std::future<std::shared_ptr<gs>> loadModelAsync(const std::string& filePath);
    void clearCache();
    void releaseUnusedModels();


private:

    ModelLoader(/* args */);
    ~ModelLoader()=default;
    std::unordered_map<std::string,std::shared_ptr<Dataset::Data>> modelCache;
    mutable std::mutex cacheMutex;
};



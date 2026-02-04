#include<modelLoader.hpp>
#include<filesystem>
ModelLoader::ModelLoader(/* args */)
{
    std::cout << "ModelLoader initialized." <<std::endl;
}


std::shared_ptr<Dataset::Data> ModelLoader::loadModel(const std::string& filePath){

    //转换为绝对路径
    std::string canonicalPath = std::filesystem::canonical(filePath).string();
    //加载资源时上锁，保证互斥，线程安全
    std::lock_guard<std::mutex> lock(cacheMutex);

    auto cacheItem = modelCache.find(canonicalPath);
    if(cacheItem != modelCache.end()){
        std::cout<< "Model cachae Hit: " << std::filesystem::path(canonicalPath).filename().string()<<std::endl;
        return cacheItem->second;
    }

    std::shared_ptr<Dataset::Data> newData = std::make_shared<Dataset::Data>(canonicalPath);

    if(!newData->load()){
        std::cout<<"load model failed\n";
        return nullptr;
    }
    std::cout<<"load model successed\n";
    modelCache[canonicalPath] = newData;
    return newData;
}
// std::future<std::shared_ptr<gs>> ModelLoader::loadModelAsync(const std::string& filePath) {
//     std::string canonicalPath = std::filesystem::canonical(filePath).string();

//     { // quick cache check under lock
//         std::lock_guard<std::mutex> lock(cacheMutex);
//         auto it = modelCache.find(canonicalPath);
//         if (it != modelCache.end()) {
//             // return already-ready future
//             std::promise<std::shared_ptr<gs>> p;
//             p.set_value(it->second);
//             return p.get_future();
//         }
//     }

//     // launch background task
//     return std::async(std::launch::async, [this, canonicalPath]() -> std::shared_ptr<gs> {
//         auto newModel = std::make_shared<gs>(canonicalPath);
//         // propagate exceptions (std::async will do that)
//         newModel->load(); // may throw
//         {
//             std::lock_guard<std::mutex> lock(cacheMutex);
//             modelCache[canonicalPath] = newModel;
//         }
//         return newModel;
//     });
// }

void ModelLoader::clearCache(){
    modelCache.clear();
}

void ModelLoader::releaseUnusedModels(){

}
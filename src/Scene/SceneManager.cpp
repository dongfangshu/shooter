#include "SceneManager.h"
#include "Scene.h"
#include "../Debug/Debug.h"
#include "format"

Scene* SceneManager::currentScene = nullptr;
std::unordered_map<std::string, Scene*> SceneManager::sceneMap;

void SceneManager::ChangeScene(std::string name)
{
    if (currentScene != nullptr)
    {
        Debug::Log(std::format("ChangeScene {} -> {}",currentScene->GetName(),name));    
        currentScene->Clear();
    }
    currentScene = sceneMap[name];
    currentScene->Init();
    Debug::Log(std::format("Scene {} Init",name));
}
void SceneManager::Update()
{
    if (currentScene != nullptr)
    {
        currentScene->Update();
    }
}
void SceneManager::AddScene(std::string name, Scene* scene)
{
    sceneMap[name] = scene;
}
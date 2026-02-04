#include "SceneManager.h"
#include "Scene.h"
Scene* SceneManager::currentScene = nullptr;
std::unordered_map<string, Scene*> SceneManager::sceneMap;

void SceneManager::ChangeScene(string name)
{
    if (currentScene != nullptr)
    {
        currentScene->Clear();
    }
    currentScene = sceneMap[name];
    currentScene->Init();
}
void SceneManager::Update()
{
    if (currentScene != nullptr)
    {
        currentScene->Update();
    }
}
void SceneManager::AddScene(string name, Scene* scene)
{
    sceneMap[name] = scene;
}
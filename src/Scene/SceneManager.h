#pragma once
#include "Scene.h"
#include <string>
#include <unordered_map>
using std::string;
class SceneManager
{
private:
    static std::unordered_map<string, Scene*> sceneMap;
public:
    SceneManager();
    ~SceneManager();
    static void AddScene(string name, Scene* scene);
    static Scene* currentScene;
    static void ChangeScene(string name);
    void Update();
};

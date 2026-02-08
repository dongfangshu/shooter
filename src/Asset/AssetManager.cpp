#include "AssetManager.h"
#include <unordered_map>
#include "../Debug/Debug.h"
#include "SDL_image.h"
AssetManager* AssetManager::instance = nullptr;
AssetManager* AssetManager::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new AssetManager();
    }
    return instance;
}
void AssetManager::Init(SDL_Renderer* render)
{
    this->render = render;
}
SDL_Texture *AssetManager::LoadTextureAtPath(const std::string &path)
{
    // 先检查字典中是否已经加载了这个纹理
    if (textureMap.find(path) != textureMap.end())
    {
        return textureMap[path];
    }
    SDL_Texture *texture = IMG_LoadTexture(render, path.c_str());
    if (texture == NULL)
    {
        Debug::Error("IMG_LoadTexture Error: " + std::string(IMG_GetError()));
        return NULL;
    }
    // 存储到字典中
    textureMap[path] = texture;
    return texture;
}
AssetManager::AssetManager() {};
AssetManager::~AssetManager()
{
    for (auto &pair : textureMap)
    {
        SDL_DestroyTexture(pair.second);
    }
}
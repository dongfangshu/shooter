#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <unordered_map>
class AssetManager
{
private:
    static AssetManager* instance;
    SDL_Renderer* render;
     std::unordered_map<std::string, SDL_Texture *> textureMap;
     AssetManager();
     ~AssetManager();
public:
    static AssetManager* GetInstance();
    void Init(SDL_Renderer* render);
     SDL_Texture *LoadTextureAtPath(const std::string &path);

};
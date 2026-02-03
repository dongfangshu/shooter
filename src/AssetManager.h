#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <unordered_map>
class AssetManager
{
    SDL_Renderer *render;
    std::unordered_map<std::string, SDL_Texture*> textureMap;
    // 构造函数，传入渲染器
public:
    AssetManager(SDL_Renderer *renderer) : render(renderer) {}
    SDL_Texture* LoadTextureAtPath(const std::string &path);
    ~AssetManager();
};

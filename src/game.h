#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "AssetManager.h"

class Game
{
    SDL_Window *window;
    SDL_Renderer *render;
    AssetManager *assetManager;
public:
    Game(SDL_Window *window);
    ~Game();
    void Run();
    bool Init();
};
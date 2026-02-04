#include "StartScene.h"
#include "../Input/InputManager.h"
#include "../Scene/SceneManager.h"
#include "../Asset/AssetManager.h"
#include "../Debug/Debug.h"

StartScene::StartScene(SDL_Renderer* renderer)
{
    this->renderer = renderer;
    this->font = nullptr;
}

StartScene::~StartScene()
{
}

void StartScene::Init()
{
    Debug::Log("StartScene::Init() called");
    font = TTF_OpenFont("assets/font/VonwaonBitmap-16px.ttf", 48);
    if (font == nullptr)
    {
        Debug::Error("Failed to load font: " + std::string(TTF_GetError()));
        return;
    }
    Debug::Log("Font loaded successfully");
    
    InputManager* inputManager = InputManager::GetInstance();
    inputManager->RegisterKeyPress(SDLK_SPACE, []() {
        Debug::Log("SPACE pressed, changing to GameScene");
        SceneManager::ChangeScene("GameScene");
    });
    
    inputManager->RegisterKeyPress(SDLK_RETURN, []() {
        Debug::Log("RETURN pressed, changing to GameScene");
        SceneManager::ChangeScene("GameScene");
    });
    Debug::Log("Input callbacks registered");
}

void StartScene::Update()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    if (font != nullptr)
    {
        SDL_Color textColor = {255, 255, 255, 255};
        
        SDL_Surface* titleSurface = TTF_RenderUTF8_Solid(font, "太空大战", textColor);
        if (titleSurface != nullptr)
        {
            SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
            if (titleTexture != nullptr)
            {
                int titleWidth = titleSurface->w;
                int titleHeight = titleSurface->h;
                SDL_Rect titleRect = {(800 - titleWidth) / 2, 200, titleWidth, titleHeight};
                SDL_RenderCopy(renderer, titleTexture, nullptr, &titleRect);
                SDL_DestroyTexture(titleTexture);
            }
            SDL_FreeSurface(titleSurface);
        }
        
        SDL_Surface* hintSurface = TTF_RenderUTF8_Solid(font, "按下任意键进入游戏", textColor);
        if (hintSurface != nullptr)
        {
            SDL_Texture* hintTexture = SDL_CreateTextureFromSurface(renderer, hintSurface);
            if (hintTexture != nullptr)
            {
                int hintWidth = hintSurface->w;
                int hintHeight = hintSurface->h;
                SDL_Rect hintRect = {(800 - hintWidth) / 2, 400, hintWidth, hintHeight};
                SDL_RenderCopy(renderer, hintTexture, nullptr, &hintRect);
                SDL_DestroyTexture(hintTexture);
            }
            SDL_FreeSurface(hintSurface);
        }
    }
    else
    {
        Debug::Error("Font is nullptr in Update()");
    }
    
    SDL_RenderPresent(renderer);
}

void StartScene::Clear()
{
    if (font != nullptr)
    {
        TTF_CloseFont(font);
        font = nullptr;
    }
    
    InputManager* inputManager = InputManager::GetInstance();
    inputManager->UnregisterKeyPress(SDLK_SPACE);
    inputManager->UnregisterKeyPress(SDLK_RETURN);
}
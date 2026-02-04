#include "EndScene.h"
#include "../Input/InputManager.h"
#include "../Scene/SceneManager.h"
#include "../Debug/Debug.h"

EndScene::EndScene(SDL_Renderer* renderer)
{
    this->renderer = renderer;
    this->font = nullptr;
}

EndScene::~EndScene()
{
}

void EndScene::Init()
{
    font = TTF_OpenFont("assets/font/VonwaonBitmap-16px.ttf", 48);
    if (font == nullptr)
    {
        Debug::Error("Failed to load font: " + std::string(TTF_GetError()));
        return;
    }
    
    InputManager* inputManager = InputManager::GetInstance();
    inputManager->RegisterKeyPress(SDLK_SPACE, []() {
        SceneManager::ChangeScene("StartScene");
    });
    
    inputManager->RegisterKeyPress(SDLK_RETURN, []() {
        SceneManager::ChangeScene("StartScene");
    });
}

void EndScene::Update()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    if (font != nullptr)
    {
        SDL_Color textColor = {255, 255, 255, 255};
        
        SDL_Surface* titleSurface = TTF_RenderUTF8_Solid(font, "游戏结束", textColor);
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
        
        SDL_Surface* hintSurface = TTF_RenderUTF8_Solid(font, "按下任意键返回主菜单", textColor);
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
    
    SDL_RenderPresent(renderer);
}

void EndScene::Clear()
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
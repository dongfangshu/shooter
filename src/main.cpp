#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <format>
#include <chrono>
#include <filesystem> // C++17
#include "Asset/AssetManager.h"
#include <windows.h>
#include "Debug/Debug.h"
#include "game.h"
#include "Debug/StackTrace.h"
SDL_Window *window;
Game *game;

int main(int argc, char **argv)
{
    StackTrace::Initialize();
    
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    Debug::Log("当前工作目录: " + std::filesystem::current_path().string());

    // 初始化sdl
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        Debug::Error("SDL_Init Error: " + std::string(SDL_GetError()));
        return 1;
    }
    Debug::Log("SDL_Init success");
    if (TTF_Init() != 0)
    {
        Debug::Error("TTF_Init Error: " + std::string(TTF_GetError()));
        return 1;
    }
    Debug::Log("TTF_Init success");
    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (IMG_Init(imgFlags) != imgFlags)
    {
        Debug::Error("IMG_Init Error: " + std::string(IMG_GetError()));
        return 1;
    }
    Debug::Log("IMG_Init success");

    int initPosX = SDL_WINDOWPOS_UNDEFINED;
    int initPosY = SDL_WINDOWPOS_UNDEFINED;
    int width = 800;
    int height = 600;
    // 创建窗口
    window = SDL_CreateWindow(
        "shooter game",  // 窗口标题
        initPosX,        // 窗口X位置
        initPosY,        // 窗口Y位置
        width,           // 窗口宽度
        height,          // 窗口高度
        SDL_WINDOW_SHOWN // 窗口标志
    );
    if (window == NULL)
    {
        Debug::Error("SDL_CreateWindow Error: " + std::string(SDL_GetError()));
        SDL_Quit();
        return 1;
    }
    Debug::Log("SDL_CreateWindow success");
    
    game = new Game(window);
    if (!game->Init())
    {
        Debug::Error("Game Init failed");
        return 1;
    }
    game->Run();

    return 0;
}
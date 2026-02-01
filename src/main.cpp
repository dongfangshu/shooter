#define SDL_MAIN_HANDLED
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <format>
#include <chrono>
#include <filesystem>  // C++17

int FRAME = 0;
const int FRAME_RATE = 60; // 帧率
TTF_Font *font;
SDL_Renderer *render;
SDL_Window *window;
void GameLoop();
void DrawGizmos(SDL_Renderer *render);
void RenderLoop(SDL_Renderer *render);
void renderText(const std::string &text, int x, int y, SDL_Color color = {255, 255, 255, 255});

int main(int argc, char **argv)
{
        std::cout << "当前工作目录: " 
                  << std::filesystem::current_path() << std::endl;

    // 初始化sdl
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }
    printf("SDL_Init success\n");
    if (TTF_Init() != 0)
    {
        printf("TTF_Init Error: %s\n", TTF_GetError());
        return 1;
    }
    printf("TTF_Init success\n");

    
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
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    printf("SDL_CreateWindow success\n");

    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (render == NULL)
    {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(render);
        SDL_Quit();
        return 1;
    }
    printf("SDL_CreateRenderer success\n");

    font = TTF_OpenFont("assets\\font\\VonwaonBitmap-16px.ttf", 16);
    if (font == NULL)
    {
        printf("TTF_OpenFont Error: %s\n", TTF_GetError());
        return 1;
    }
    
    SDL_Event event;
    // char title[256];
    // 绘制
    while (true)
    {
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
        {
            printf("SDL_QUIT event\n");
            break;
        }
        FRAME++;
        auto start = std::chrono::high_resolution_clock::now();
        GameLoop();
        RenderLoop(render);
        DrawGizmos(render);
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        if (duration.count() < 1000 / FRAME_RATE)
        {
            SDL_Delay(1000 / FRAME_RATE - duration.count());
        }
        SDL_SetRenderDrawColor(render, 0, 0, 0, 255); // 用默认颜色清屏
        SDL_RenderClear(render);
    }
    // 清理SDL资源
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();
    printf("SDL_CLEAR success\n");

    return 0;
}
void GameLoop()
{
}
void DrawGizmos(SDL_Renderer *render)
{
    SDL_SetRenderDrawColor(render, 70, 70, 70, 255);
    auto title = std::format("frame:{}", FRAME);
    renderText(title, 10, 10);
    SDL_Rect rect = {0, 0, 300, 100};
    SDL_RenderFillRect(render, &rect);
}
void RenderLoop(SDL_Renderer *render)
{
    SDL_SetRenderDrawColor(render, 255, 0, 0, 255);
    // 对窗口宽度取余
    int windowWidth = 800;
    SDL_Rect rect = {FRAME % windowWidth, 100, 100, 100};
    SDL_RenderFillRect(render, &rect);
    SDL_RenderPresent(render);
}
SDL_Texture *createTextTexture(const std::string &text,
                               SDL_Color color)
{
    // 渲染文字到surface
    SDL_Surface *textSurface = TTF_RenderUTF8_Blended(font, text.c_str(), color);
    if (!textSurface)
    {
        std::cerr << "文字Surface创建失败: " << TTF_GetError() << std::endl;
        return nullptr;
    }

    // 创建纹理
    SDL_Texture *texture = SDL_CreateTextureFromSurface(render, textSurface);
    SDL_FreeSurface(textSurface); // 释放surface

    return texture;
}
void renderText(const std::string &text, int x, int y,
                SDL_Color color)
{
    SDL_Texture *texture = createTextTexture(text, color);
    if (!texture)
        return;

    // 获取纹理尺寸
    int width, height;
    SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);

    // 设置渲染位置
    SDL_Rect destRect = {x, y, width, height};

    // 渲染纹理
    SDL_RenderCopy(render, texture, nullptr, &destRect);

    // 释放纹理
    SDL_DestroyTexture(texture);
}
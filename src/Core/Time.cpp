#include "Time.h"
#include <SDL2/SDL.h>

Time* Time::instance = nullptr;

Time::Time()
    : deltaTime(0.0f)
    , totalTime(0.0f)
    , frameCount(0)
    , currentFPS(0.0f)
    , targetFrameRate(60)
    , lastFrameTime(0)
    , frameStartTime(0)
    , fpsTimer(0.0f)
    , fpsCounter(0)
{
}

Time::~Time()
{
}

Time* Time::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new Time();
    }
    return instance;
}

void Time::DestroyInstance()
{
    if (instance != nullptr)
    {
        delete instance;
        instance = nullptr;
    }
}

void Time::Update()
{
    frameStartTime = SDL_GetTicks64();
    
    if (lastFrameTime > 0)
    {
        // 计算 deltaTime（转换为秒）
        deltaTime = (frameStartTime - lastFrameTime) / 1000.0f;
    }
    else
    {
        // 第一帧
        deltaTime = 0.0f;
    }
    
    // 限制 deltaTime 最大值，防止在卡顿或调试时产生过大的时间步
    // 这可以防止物理和移动出现跳跃
    const float MAX_DELTA_TIME = 0.1f; // 最大 100ms
    if (deltaTime > MAX_DELTA_TIME)
    {
        deltaTime = MAX_DELTA_TIME;
    }
    
    // 更新总时间
    totalTime += deltaTime;
    
    // 更新帧计数
    frameCount++;
    
    // 计算 FPS
    fpsTimer += deltaTime;
    fpsCounter++;
    if (fpsTimer >= 1.0f)
    {
        currentFPS = fpsCounter / fpsTimer;
        fpsTimer = 0.0f;
        fpsCounter = 0;
    }
    
    lastFrameTime = frameStartTime;
}

void Time::CapFrameRate()
{
    if (targetFrameRate <= 0)
        return;
    
    unsigned long long currentTime = SDL_GetTicks64();
    unsigned long long frameTime = currentTime - frameStartTime;
    unsigned long long targetFrameTime = 1000ULL / targetFrameRate;
    
    if (frameTime < targetFrameTime)
    {
        SDL_Delay(static_cast<Uint32>(targetFrameTime - frameTime));
    }
}

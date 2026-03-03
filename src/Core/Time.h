#pragma once

class Time
{
public:
    // 获取 Time 单例实例
    static Time* GetInstance();
    
    // 销毁单例实例
    static void DestroyInstance();
    
    // 每帧调用，更新时间数据
    void Update();
    
    // 获取上一帧的耗时（秒）
    float GetDeltaTime() const { return deltaTime; }
    
    // 获取从游戏开始到现在的总时间（秒）
    float GetTime() const { return totalTime; }
    
    // 获取当前帧数
    unsigned int GetFrameCount() const { return frameCount; }
    
    // 获取当前 FPS
    float GetFPS() const { return currentFPS; }
    
    // 设置目标帧率（用于限制帧率，0 表示不限制）
    void SetTargetFrameRate(int fps) { targetFrameRate = fps; }
    
    // 获取目标帧率
    int GetTargetFrameRate() const { return targetFrameRate; }
    
    // 限制帧率（如果需要）
    void CapFrameRate();

private:
    Time();
    ~Time();
    
    static Time* instance;
    
    float deltaTime;           // 上一帧耗时（秒）
    float totalTime;           // 总运行时间（秒）
    unsigned int frameCount;   // 帧计数
    float currentFPS;          // 当前 FPS
    int targetFrameRate;       // 目标帧率
    
    // 用于计算的时间戳
    unsigned long long lastFrameTime;  // 上一帧的时间戳（毫秒）
    unsigned long long frameStartTime; // 当前帧开始时间戳（毫秒）
    
    // FPS 计算
    float fpsTimer;
    int fpsCounter;
};

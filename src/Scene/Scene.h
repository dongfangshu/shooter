#pragma once
#include <vector>
#include <string>
class Canvas;

// 宏定义：自动生成GetName方法
#define DEFINE_SCENE_NAME(ClassName) \
    std::string GetName() override { return #ClassName; }

#include <SDL2/SDL.h>

class Scene
{
public:
    Scene() {}
    virtual ~Scene() {}
    virtual std::string GetName() { return ""; };
    virtual void Update() = 0;
    virtual void Render(SDL_Renderer* renderer) = 0;
    virtual void Init() = 0;
    virtual void Clear() = 0;
    virtual Canvas *GetCanvas() { return nullptr; }
};

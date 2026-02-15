#pragma once
#include <vector>

class Canvas;

class Scene {
public:
    Scene() {}
    virtual ~Scene() {}
    virtual void Update() = 0;
    virtual void Init() = 0;
    virtual void Clear() = 0;
    virtual Canvas* GetCanvas() { return nullptr; }
};

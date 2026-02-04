#pragma once
class Scene
{
private:
    /* data */
public:
    Scene() {}
    virtual ~Scene() {}
    virtual void Update() = 0;
    virtual void Init() = 0;
    virtual void Clear() = 0;
};

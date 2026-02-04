#pragma once
class Scene
{
private:
    /* data */
public:
    Scene(/* args */);
    ~Scene();
    virtual void Update();
    virtual void Init();
    virtual void Clear();
};

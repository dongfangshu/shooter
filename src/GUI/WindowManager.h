#include <SDL2/SDL_ttf.h>
class WindowManager
{
private:
    /* data */
public:
    TTF_Font* GetFont(std::string fontName);
    WindowManager(/* args */);
    ~WindowManager();
};

WindowManager::WindowManager(/* args */)
{
}

WindowManager::~WindowManager()
{
}

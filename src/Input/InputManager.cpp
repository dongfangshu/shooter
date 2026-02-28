#include "InputManager.h"
#include "../Debug/Debug.h"
#include <format>

InputManager* InputManager::instance = nullptr;

InputManager::InputManager()
{
#ifdef _WIN32
    originalKeyboardLayout = NULL;
#endif
}

InputManager::~InputManager()
{
}

InputManager* InputManager::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new InputManager();
    }
    return instance;
}

void InputManager::DestroyInstance()
{
    if (instance != nullptr)
    {
        delete instance;
        instance = nullptr;
    }
}

void InputManager::HandleEvent(const SDL_Event& event)
{
    #ifdef ENABLE_INPUT_LOG 
        Debug::Log(std::format("InputManager::HandleEvent - SDL_Event type: {}", event.type));
    #endif
    if (event.type == SDL_KEYDOWN)
    {
        SDL_Keycode key = event.key.keysym.sym;
        // eventQueue.push(event);
        keyStates[key] = true;
        
        auto it = keyPressCallbacks.find(key);
        if (it != keyPressCallbacks.end())
        {
            it->second();
        }
    }
    else if (event.type == SDL_KEYUP)
    {
        SDL_Keycode key = event.key.keysym.sym;
        keyStates[key] = false;
    }
    else if (event.type == SDL_WINDOWEVENT)
    {
        // 窗口获得焦点时，切换到英文输入法
        if (event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED)
        {
            SwitchToEnglishInputMethod();
        }
        // 窗口失去焦点时，恢复原始输入法
        else if (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST)
        {
            RestoreOriginalInputMethod();
        }
        // 窗口关闭时，恢复原始输入法
        else if (event.window.event == SDL_WINDOWEVENT_CLOSE)
        {
            RestoreOriginalInputMethod();
        }
    }
}

void InputManager::SwitchToEnglishInputMethod()
{
#ifdef _WIN32
    // 如果已经保存了原始布局且当前已经是英文，则不重复切换
    if (originalKeyboardLayout != NULL)
    {
        char klid[KL_NAMELENGTH];
        if (GetKeyboardLayoutNameA(klid) && strstr(klid, "00000409") != nullptr)
        {
            return; // 已经是英文布局
        }
    }
    else
    {
        // 首次切换时保存原始键盘布局
        originalKeyboardLayout = GetKeyboardLayout(GetCurrentThreadId());
    }
    
    // 获取当前线程的键盘布局
    HKL hkl = GetKeyboardLayout(GetCurrentThreadId());
    
    // 检查当前是否是英文键盘布局
    // 中文键盘布局的日文部分是 0xE0010804 (微软拼音) 或 0xE0010404 (搜狗拼音) 等
    // 低位 word 是语言 ID，高位 word 是键盘布局 ID
    WORD langId = LOWORD(hkl);
    
    // 如果是中文语言 (简体中文: 0x0804, 繁体中文: 0x0404)
    if (langId == 0x0804 || langId == 0x0404)
    {
        // 尝试激活英文键盘布局
        // 枚举系统中的键盘布局并切换到第一个英文布局
        HKL hklEnum = NULL;
        char klid[KL_NAMELENGTH];
        
        if (GetKeyboardLayoutNameA(klid))
        {
            // 检查当前布局名称是否已经是英文
            if (strstr(klid, "00000409") == nullptr) // 不是美式英文
            {
                // 尝试加载美式英文键盘布局
                HKL hklEnglish = LoadKeyboardLayoutA("00000409", KLF_ACTIVATE);
                if (hklEnglish != NULL)
                {
                    Debug::Log("Switched to English input method");
                }
            }
        }
    }
#endif
}

void InputManager::RestoreOriginalInputMethod()
{
#ifdef _WIN32
    // 恢复原始键盘布局
    if (originalKeyboardLayout != NULL)
    {
        ActivateKeyboardLayout(originalKeyboardLayout, KLF_SETFORPROCESS);
        Debug::Log("Restored original input method");
    }
#endif
}

void InputManager::Initialize()
{
    // 初始化时切换到英文输入法
    SwitchToEnglishInputMethod();
}

void InputManager::RegisterKeyPress(SDL_Keycode key, std::function<void()> callback)
{
    keyPressCallbacks[key] = callback;
}

void InputManager::UnregisterKeyPress(SDL_Keycode key)
{
    keyPressCallbacks.erase(key);
}

bool InputManager::IsKeyPressed(SDL_Keycode key) const
{
    auto it = keyStates.find(key);
    if (it != keyStates.end())
    {
        return it->second;
    }
    return false;
}

void InputManager::Update()
{
}
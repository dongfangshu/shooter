#include "StartScene.h"
#include "../Input/InputManager.h"
#include "../Scene/SceneManager.h"
#include "../Asset/AssetManager.h"
#include "../GUI/UI/Canvas.h"
#include "../GUI/UI/GText.h"
#include "../GUI/UI/GButton.h"
#include "../GUI/UI/GImage.h"
#include "../GUI/TextUtil.h"
#include "../Debug/Debug.h"
#include <SDL2/SDL_ttf.h>

StartScene::StartScene(SDL_Renderer* renderer)
    : renderer(renderer){
}

StartScene::~StartScene() {
}

Canvas* StartScene::GetCanvas() {
    return canvas;
}

void StartScene::Init() {
    Debug::Log("StartScene::Init() called");

    canvas = new Canvas(renderer, SCREEN_W, SCREEN_H);
    Debug::Log("Canvas called");

    SDL_Texture* bgTex = AssetManager::GetInstance()->LoadTextureAtPath("assets/image/bg.png");
    if (bgTex) {
        GImage* bg = new GImage(bgTex);
        bg->SetWidth(SCREEN_W);
        bg->SetHeight(SCREEN_H);
        canvas->AddChild(bg);
    }

    SDL_Color textColor = {255, 255, 255, 255};
    GText* titleText = new GText("太空大战");
    canvas->AddChild(titleText);
    titleText->SetPosition({(SCREEN_W - titleText->GetRect().w) / 2, 120});
    titleText->SetColor(textColor);

    // GText* hintText = new GText("按下任意键进入游戏");
    // hintText->SetPosition({(SCREEN_W - hintText->GetRect().w) / 2, 520});
    // hintText->SetColor(textColor);
    // canvas->AddChild(hintText);

    

    int btnWidth = 200;
    int btnHeight = 50;
    GButton* startBtn = new GButton(btnWidth, btnHeight);
    canvas->AddChild(startBtn);
    startBtn->label->SetColor(textColor);
    startBtn->SetText("开始游戏");
    startBtn->SetPosition({(SCREEN_W - btnWidth) / 2, 480});
    startBtn->SetOnClick([]() {
        Debug::Log("Start button clicked, changing to GameScene");
        SceneManager::ChangeScene("GameScene");
    });

    // InputManager* inputManager = InputManager::GetInstance();
    // inputManager->RegisterKeyPress(SDLK_SPACE, []() {
    //     Debug::Log("SPACE pressed, changing to GameScene");
    //     SceneManager::ChangeScene("GameScene");
    // });
    // inputManager->RegisterKeyPress(SDLK_RETURN, []() {
    //     Debug::Log("RETURN pressed, changing to GameScene");
    //     SceneManager::ChangeScene("GameScene");
    // });
    Debug::Log("StartScene UI and input registered");
}

void StartScene::Update() {
}

void StartScene::Clear() {
    // if (canvas != nullptr) {
    //     delete canvas;
    //     canvas = nullptr;
    // }
    //if (font != nullptr) {
    //    TTF_CloseFont(font);
    //    font = nullptr;
    //}
    InputManager* inputManager = InputManager::GetInstance();
    inputManager->UnregisterKeyPress(SDLK_SPACE);
    inputManager->UnregisterKeyPress(SDLK_RETURN);
}

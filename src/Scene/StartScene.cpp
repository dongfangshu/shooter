#include "StartScene.h"
#include "../Input/InputManager.h"
#include "../Scene/SceneManager.h"
#include "../Asset/AssetManager.h"
#include "../GUI/UI/Stage.h"
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

Stage* StartScene::GetStage() {
    return stage;
}

void StartScene::Init() {
    Debug::Log("StartScene::Init() called");

    stage = new Stage(renderer, SCREEN_W, SCREEN_H);
    Debug::Log("Stage called");

    SDL_Texture* bgTex = AssetManager::GetInstance()->LoadTextureAtPath("assets/image/bg.png");
    if (bgTex) {
        GImage* bg = new GImage(bgTex);
        bg->SetRect({0, 0, SCREEN_W, SCREEN_H});
        stage->AddChild(bg);
    }

    SDL_Color textColor = {255, 255, 255, 255};
    GText* titleText = new GText("太空大战");
    titleText->SetColor(textColor);
    stage->AddChild(titleText);

    GText* hintText = new GText("按下任意键进入游戏");
    hintText->SetColor(textColor);
    stage->AddChild(hintText);

    

    SDL_Rect btnRect = {(SCREEN_W - 200) / 2, 480, 200, 50};
    GButton* startBtn = new GButton(btnRect);
    startBtn->SetText("开始游戏");
    startBtn->SetOnClick([]() {
        Debug::Log("Start button clicked, changing to GameScene");
        SceneManager::ChangeScene("GameScene");
    });
    stage->AddChild(startBtn);

    InputManager* inputManager = InputManager::GetInstance();
    inputManager->RegisterKeyPress(SDLK_SPACE, []() {
        Debug::Log("SPACE pressed, changing to GameScene");
        SceneManager::ChangeScene("GameScene");
    });
    inputManager->RegisterKeyPress(SDLK_RETURN, []() {
        Debug::Log("RETURN pressed, changing to GameScene");
        SceneManager::ChangeScene("GameScene");
    });
    Debug::Log("StartScene UI and input registered");
}

void StartScene::Update() {
}

void StartScene::Clear() {
    if (stage != nullptr) {
        delete stage;
        stage = nullptr;
    }
    //if (font != nullptr) {
    //    TTF_CloseFont(font);
    //    font = nullptr;
    //}
    InputManager* inputManager = InputManager::GetInstance();
    inputManager->UnregisterKeyPress(SDLK_SPACE);
    inputManager->UnregisterKeyPress(SDLK_RETURN);
}

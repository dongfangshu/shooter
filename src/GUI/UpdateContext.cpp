#include "UpdateContext.h"

UpdateContext::UpdateContext(SDL_Renderer* renderer) : renderer(renderer) {
}

UpdateContext::~UpdateContext() {
}

void UpdateContext::Begin() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void UpdateContext::End() {
    for (auto& callback : renderCallbacks) {
        callback(renderer);
    }
    renderCallbacks.clear();
    SDL_RenderPresent(renderer);
}

void UpdateContext::AddRenderCallback(std::function<void(SDL_Renderer*)> callback) {
    renderCallbacks.push_back(callback);
}

void UpdateContext::ClearCallbacks() {
    renderCallbacks.clear();
}

SDL_Renderer* UpdateContext::GetRenderer() const {
    return renderer;
}
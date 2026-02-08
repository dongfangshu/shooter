#pragma once
#include <SDL2/SDL_ttf.h>

namespace TextUtil {
    // 测量 UTF8 文本在给定字体下的像素宽高，outW/outH 可为 nullptr
    void MeasureText(TTF_Font* font, const char* text, int* outW, int* outH);
}

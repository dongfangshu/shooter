#include "TextUtil.h"

namespace TextUtil {
void MeasureText(TTF_Font* font, const char* text, int* outW, int* outH) {
    if (!font || !text) {
        if (outW) *outW = 0;
        if (outH) *outH = 0;
        return;
    }
    int w = 0, h = 0;
    TTF_SizeUTF8(font, text, &w, &h);
    if (outW) *outW = w;
    if (outH) *outH = h;
}
}

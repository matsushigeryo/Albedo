#include "global.h"

bool is_running = false;
float current_FPS = 0.0f;
int render_buffer = -1;

// üŒ`•âŠÔŠÖ”
float LerpF(float start, float end, float t) {
    return start + (end - start) * t;
}

// ‹tüŒ`•âŠÔŠÖ”
float InvLerpF(float start, float end, float v) {
    if (std::abs(end - start) < 1e-6f) { // ƒ[ƒœZ‚Ì‘Îô
        return 0.0f;
    }
    return (v - start) / (end - start);
}

// ‚»‚ÌÀ•W‚ª‰æ–Ê“à‚©H
bool OnScreen(int x, int y) {
    return x >= 0 && x < kWindowWidth && y >= 0 && y < kWindowHeight;
}
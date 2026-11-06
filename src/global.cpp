#include "global.h"
#include <Windows.h>
#include <Psapi.h>

bool is_running = false;
float current_FPS = 0.0f;
int render_buffer = -1;

// 線形補間関数
float LerpF(float start, float end, float t) {
    return start + (end - start) * t;
}

// 逆線形補間関数
float InvLerpF(float start, float end, float v) {
    if (std::abs(end - start) < 1e-6f) { // ゼロ除算の対策
        return 0.0f;
    }
    return (v - start) / (end - start);
}

// イージング関数群
float EaseInQuad(float x) {
    return x * x;
}
float EaseOutQuad(float x) {
    return 1.0f - (1.0f - x) * (1.0f - x);
}

// 0.0～1.0に正規化されたランダムな浮動小数点値を取得する
float GetRandomNormalized() {
    return static_cast<float>(rand()) / RAND_MAX;
}

// その座標が画面内か？
bool OnScreen(int x, int y) {
    return x >= 0 && x < kWindowWidth && y >= 0 && y < kWindowHeight;
}

// 現在のプロセスのメモリ使用量を取得する(バイト単位)
unsigned long GetWindowsMemoryUsage() {
    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&pmc), sizeof(pmc));
    return static_cast<unsigned long>(pmc.PrivateUsage);
}
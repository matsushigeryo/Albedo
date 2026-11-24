#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <memory>

constexpr float kPi = 3.14159265f;
constexpr float kSqrt2 = 1.414213562f;

constexpr const char *kWindowTitle = "DXƒ‰ƒCƒuƒ‰ƒŠ";
constexpr long long kIdealFrameDuration = 16666;
constexpr int kWindowWidth = 960;
constexpr int kWindowHeight = 720;
constexpr int kGraphicWidth = 320;
constexpr int kGraphicHeight = 240;

extern bool is_running;
extern float current_FPS;
extern int render_buffer;

float LerpF(float start, float end, float t);
float InvLerpF(float start, float end, float v);
float EaseInQuad(float x);
float EaseOutQuad(float x);
float EaseInCubic(float x);
float EaseOutCubic(float x);
float GetDistance(float ax, float ay, float bx, float by);
float DegreeToRadian(float degree);
float RadianToDegree(float radian);
bool OnScreen(int x, int y);
unsigned long GetWindowsMemoryUsage();

#endif
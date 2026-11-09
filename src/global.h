#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <memory>
#include "asset.h"
#include "input.h"
#include "log.h"
#include "camera.h"
#include "tilemap.h"
#include "player.h"
#include "player_bullet.h"
#include "enemy.h"
#include "enemy_bullet.h"
#include "enemy_script.h"
#include "effect.h"

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
float GetRandomNormalized();
bool OnScreen(int x, int y);
unsigned long GetWindowsMemoryUsage();

namespace game {
    extern int game_time;
    extern Asset asset;
    extern Input input;
    extern Log log;
    extern Camera camera;
    extern std::unique_ptr<TileMap> tilemap_ptr;
    extern std::unique_ptr<Player> player_ptr;
    extern std::unique_ptr<PlayerBulletManager> player_bullet_manager_ptr;
    extern std::unique_ptr<EnemyManager> enemy_manager_ptr;
    extern std::unique_ptr<EnemyBulletManager> enemy_bullet_manager_ptr;
    extern std::unique_ptr<EnemyScriptManager> enemy_script_manager_ptr;
    extern std::unique_ptr<EffectManager> effect_manager_ptr;
}

#endif
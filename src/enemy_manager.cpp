#include "enemy_manager.h"
#include <iostream>
#include <DxLib.h>
#include "global.h"

namespace game {
    void EnemyManager::Init() {
        for (auto &enemy : enemies_) {
            enemy.is_active_ = false;
        }
    }

    void EnemyManager::Update() {
        for (auto& enemy : enemies_) {
            if (enemy.is_active_) {
                enemy.on_damage_timer_++;
            }
        }
    }

    void EnemyManager::Draw() {
        int image_handle = -1;
        for (const auto &enemy : enemies_) {
            if (enemy.is_active_) {
                image_handle = asset.GetImageAsset(kEnemySpriteImageIDs[static_cast<size_t>(enemy.sprite_)]);
                DrawRotaGraphF(enemy.x_, enemy.y_, 1.0, 0.0, image_handle, true);
            }
        }
    }

    bool EnemyManager::Spawn(EnemySprite sprite, float x, float y) {
        for (auto &enemy : enemies_) {
            if (!enemy.is_active_) { // ”z—ñ‚©‚ç‹ó‚«‚ğŒ©‚Â‚¯‚Ä“G‚ğ’Ç‰Á
                enemy.is_active_ = true;
                enemy.sprite_ = sprite;
                enemy.x_ = x;
                enemy.y_ = y;
                enemy.elapsed_time_ = 0;
                return true;
            }
        }

        // “G‚ª’Ç‰Á‚Å‚«‚È‚©‚Á‚½ê‡
        std::cerr << "“G‚Ì’Ç‰Á‚É¸”s‚µ‚Ü‚µ‚½B" << std::endl;
        return false;
    }

    void OnDamage(Enemy& enemy) {
        if (enemy.is_active_) {
            enemy.on_damage_timer_ = 0;
        }
    }

    std::array<Enemy, EnemyManager::kMaxNum> EnemyManager::GetEnemies() {
        return enemies_;
    }
}
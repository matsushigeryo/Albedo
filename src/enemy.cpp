#include "enemy.h"
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
        for (auto &enemy : enemies_) {
            if (enemy.is_active_) {
                enemy.on_damage_timer_++;
                enemy.y_ += 1.0f;
                if (enemy.y_ > kGraphicHeight + 32) {
                    enemy.is_active_ = false;
                }
            }
        }
    }

    void EnemyManager::Draw() {
        int image_handle = -1;
        float flash_add = 0.0f;

        for (const auto &enemy : enemies_) {
            if (enemy.is_active_) {
                image_handle = asset.GetImageAsset(kEnemySpriteImageIDs[static_cast<size_t>(enemy.sprite_)]); // •`‰æ‚·‚é‰æ‘œƒnƒ“ƒhƒ‹
                DrawRotaGraphF(enemy.x_, enemy.y_, 1.0, 0.0, image_handle, true);                             // ƒXƒvƒ‰ƒCƒg•`‰æ

                // ã‚©‚ç”’“h‚è‚ÌƒXƒvƒ‰ƒCƒg‚ğ•`‰æ‚µ‚Ä“_–Å‰‰o‚ğs‚¤
                if (enemy.on_damage_timer_ >= 0 && enemy.on_damage_timer_ <= kEnemyOnDamageFlashEffectLength) {
                    flash_add = InvLerpF(kEnemyOnDamageFlashEffectLength, 0, enemy.on_damage_timer_);
                    SetIgnoreDrawGraphColor(true);
                    SetDrawBlendMode(DX_BLENDMODE_ADD, flash_add * 255);
                    DrawRotaGraphF(enemy.x_, enemy.y_, 1.0, 0.0, image_handle, true);
                    SetIgnoreDrawGraphColor(false);
                    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
                }
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
                enemy.on_damage_timer_ = kEnemyOnDamageFlashEffectLength + 1;
                enemy.life_ = 10; // ‰¼‚ÅHP10‚Æ‚·‚é
                return true;
            }
        }

        // “G‚ª’Ç‰Á‚Å‚«‚È‚©‚Á‚½ê‡
        std::cerr << "“G‚Ì’Ç‰Á‚É¸”s‚µ‚Ü‚µ‚½B" << std::endl;
        return false;
    }

    // “G‚Ì€–S
    void EnemyManager::Die(Enemy &enemy) {
        if (enemy.is_active_) {
            enemy.is_active_ = false; // Á‹
            // Œ‚”jƒGƒtƒFƒNƒg
            for (int i = 0; i < 5; ++i) {
                effect_manager_ptr->Spawn(EffectType::EnemyDead, enemy.x_, enemy.y_);
            }
        }
    }

    // ƒvƒŒƒCƒ„[’e‚É“–‚½‚Á‚½‚Æ‚«
    void EnemyManager::OnHitPlayerBullet(Enemy &enemy) {
        if (enemy.is_active_) {
            enemy.on_damage_timer_ = 0;
            enemy.life_--;
            if (enemy.life_ <= 0) {
                Die(enemy);
            }
        }
    }

    // “G‚Ì”z—ñ‚ğæ“¾‚·‚é
    std::array<Enemy, EnemyManager::kMaxNum> &EnemyManager::GetEnemies() {
        return enemies_;
    }
}
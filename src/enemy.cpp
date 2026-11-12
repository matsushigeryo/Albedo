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

                // スクリプト実行
                if (enemy.script_runner_) {
                    enemy_script::EnemyScriptRunnerContext context{enemy.x_, enemy.y_, enemy.is_active_};
                    enemy.script_runner_->Update(context);
                }

                // タイマー更新
                enemy.elapsed_time_++;
                enemy.on_damage_timer_++;
            }
        }
    }

    void EnemyManager::Draw() {
        int image_handle = -1;
        float flash_add = 0.0f;

        for (const auto &enemy : enemies_) {
            if (enemy.is_active_) {

                // 描画座標を算出
                const float draw_x = enemy.x_;
                const float draw_y = enemy.y_;

                // スプライト描画
                image_handle = asset.GetImageAsset(kEnemySpriteImageIDs[static_cast<size_t>(enemy.sprite_)]); // 描画する画像ハンドル
                DrawRotaGraphF(draw_x, draw_y, 1.0, 0.0, image_handle, true);                                 // スプライト描画

                // 上から白塗りのスプライトを描画して点滅演出を行う
                if (enemy.on_damage_timer_ >= 0 && enemy.on_damage_timer_ <= kEnemyOnDamageFlashEffectLength) {
                    flash_add = InvLerpF(kEnemyOnDamageFlashEffectLength, 0, enemy.on_damage_timer_);
                    SetIgnoreDrawGraphColor(true);
                    SetDrawBlendMode(DX_BLENDMODE_ADD, flash_add * 255);
                    DrawRotaGraphF(draw_x, draw_y, 1.0, 0.0, image_handle, true);
                    SetIgnoreDrawGraphColor(false);
                    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
                }
            }
        }
    }

    bool EnemyManager::Spawn(EnemySprite sprite, float x, float y, std::string script_name) {
        for (auto &enemy : enemies_) {
            if (!enemy.is_active_) { // 配列から空きを見つけて敵を追加

                enemy.is_active_ = true;
                enemy.sprite_ = sprite;
                enemy.x_ = x;
                enemy.y_ = y;
                enemy.elapsed_time_ = 0;
                enemy.on_damage_timer_ = kEnemyOnDamageFlashEffectLength + 1;
                enemy.life_ = 8; // 仮でHP8とする

                // スクリプトを割り当て
                if (script_name != "") {
                    const std::string full_path = "asset/data/" + script_name + ".xml";
                    enemy.script_runner_ = std::make_unique<enemy_script::EnemyScriptRunner>(enemy_script_manager_ptr->Get(full_path));
                }

                return true;
            }
        }

        // 敵が追加できなかった場合
        log.Print("EnemyManager: 敵のスポーンに失敗しました");
        return false;
    }

    // 敵の死亡
    void EnemyManager::Die(Enemy &enemy) {
        if (enemy.is_active_) {
            enemy.is_active_ = false; // 消去
            // 撃破エフェクト
            for (int i = 0; i < 5; ++i) {
                effect_manager_ptr->Spawn(EffectType::EnemyDead, enemy.x_, enemy.y_);
            }
        }
    }

    // プレイヤー弾に当たったとき
    void EnemyManager::OnHitPlayerBullet(Enemy &enemy) {
        if (enemy.is_active_) {
            enemy.on_damage_timer_ = 0;
            enemy.life_--;
            if (enemy.life_ <= 0) {
                Die(enemy);
            }
        }
    }

    // 敵の配列を取得する
    std::array<Enemy, EnemyManager::kMaxNum> &EnemyManager::GetEnemies() {
        return enemies_;
    }

    // アクティブな敵の数を取得する
    size_t EnemyManager::GetActiveEnemyCount() const {
        size_t count = 0;
        for (const auto &enemy : enemies_) {
            if (enemy.is_active_) {
                count++;
            }
        }
        return count;
    }
}
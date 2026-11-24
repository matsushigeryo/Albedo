#include "enemy.h"
#include <iostream>
#include <DxLib.h>
#include "effect.h"
#include "game.h"
#include "global.h"
#include "item.h"
#include "log.h"
#include "xoshiro.h"

namespace game {

    // 敵のスポーン
    Enemy *EnemyManager::Spawn(EnemySprite sprite, float x, float y, std::string script_name) {
        auto *enemy = Spawn(sprite, x, y);

        if (enemy) {
            // 固有のメンバ変数を設定
            enemy->on_damage_timer_ = kEnemyOnDamageFlashEffectLength + 1;
            enemy->life_ = 8;
            // スクリプトを割り当て
            if (script_name != "") {
                const std::string full_path = "asset/data/" + script_name + ".xml";
                enemy->script_runner_ = std::make_unique<EnemyScriptRunner>(g_game.enemy_script_manager->Get(full_path));
            }
        } else {
            // 敵が追加できなかった場合
            g_game.log->Print("EnemyManager: 敵のスポーンに失敗しました");
        }

        return enemy;
    }

    // 敵の死亡
    void EnemyManager::Die(Enemy &enemy) {
        if (enemy.is_active_) {
            enemy.is_active_ = false; // 消去
            // 撃破エフェクト
            for (int i = 0; i < 5; ++i) {
                g_game.effect_manager->Spawn(EffectType::EnemyDead, enemy.x_, enemy.y_);
            }
            // アイテム出現
            for (int i = 0; i < 5; ++i) {
                const float ofs_x = LerpF(-8, 8, g_game.xoshiro->GetRandomNormalized());
                const float ofs_y = LerpF(-8, 8, g_game.xoshiro->GetRandomNormalized());
                g_game.item_manager->Spawn(ItemType::Coin, enemy.x_ + ofs_x, enemy.y_ + ofs_y);
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

    // 固有の更新処理
    void EnemyManager::UniqueUpdate(Enemy &enemy) {
        // スクリプト実行
        if (enemy.script_runner_) {
            EnemyScriptRunnerContext context{enemy.x_, enemy.y_, enemy.is_active_};
            enemy.script_runner_->Update(context);
        }
        // タイマー更新
        enemy.on_damage_timer_++;
    }

    // 固有の描画処理
    void EnemyManager::UniqueDraw(Enemy &enemy, const SpriteProperty &prop, int image_handle, int src_x, int image_height) {
        // 描画座標を算出
        const float draw_x = enemy.x_;
        const float draw_y = enemy.y_;
        // スプライト描画
        DrawRectRotaGraphF(draw_x, draw_y, src_x, 0, prop.image_width_, image_height, 1.0, 0.0, image_handle, true);
        // 上から白塗りのスプライトを描画して点滅演出を行う
        if (enemy.on_damage_timer_ >= 0 && enemy.on_damage_timer_ <= kEnemyOnDamageFlashEffectLength) {
            const float flash_add = InvLerpF(kEnemyOnDamageFlashEffectLength, 0, enemy.on_damage_timer_);
            SetIgnoreDrawGraphColor(true);
            SetDrawBlendMode(DX_BLENDMODE_ADD, flash_add * 255);
            DrawRectRotaGraphF(draw_x, draw_y, src_x, 0, prop.image_width_, image_height, 1.0, 0.0, image_handle, true);
            SetIgnoreDrawGraphColor(false);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        }
    }
}
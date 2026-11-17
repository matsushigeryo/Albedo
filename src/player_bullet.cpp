#include "player_bullet.h"
#include <iostream>
#include <DxLib.h>
#include "asset.h"
#include "effect.h"
#include "game.h"
#include "global.h"

namespace game {

    // 敵に当たったとき
    void PlayerBulletManager::OnHitEnemy(PlayerBullet &bullet) {
        bullet.is_active_ = false;                                                           // 消去
        g_game.effect_manager->Spawn(EffectType::PlayerBulletSparkle, bullet.x_, bullet.y_); // エフェクト発生
    }

    // 固有のスポーン処理
    void PlayerBulletManager::UniqueSpawn(PlayerBullet &bullet) {
        bullet.vy_ = kBulletSpeed * -1.0f; // 移動速度を設定
    }

    // 固有の更新処理
    void PlayerBulletManager::UniqueUpdate(PlayerBullet &bullet) {
        if (bullet.y_ < -32) { // 画面外に出た弾を削除
            bullet.is_active_ = false;
        }
    }

    // 固有の描画処理
    void PlayerBulletManager::UniqueDraw(PlayerBullet &bullet, const SpriteProperty &prop, int image_handle, int src_x, int image_height) {

        // 射出直後から時間経過でフェードインする
        float alpha = 1.0f;
        if (bullet.elapsed_time_ >= 0 && bullet.elapsed_time_ <= 8) {
            alpha = InvLerpF(0, 8, bullet.elapsed_time_);
            alpha = LerpF(0, 180, alpha);
        }

        // スプライト描画
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha * 255);
        DrawRectRotaGraphF(bullet.x_, bullet.y_, src_x, 0, prop.image_width_, image_height, 1.0, 0.0, image_handle, true);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }
}
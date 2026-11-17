#include "player_bullet.h"
#include <iostream>
#include <DxLib.h>
#include "asset.h"
#include "effect.h"
#include "game.h"
#include "global.h"

namespace game {

    void PlayerBulletManager::Draw() {
        float alpha = 1.0f;
        const auto &properties = GetSpriteProperties();

        for (const auto &bullet : GetEntities()) {
            if (!bullet.is_active_) continue;

            // 射出直後から時間経過でフェードインする
            if (bullet.elapsed_time_ >= 0 && bullet.elapsed_time_ <= 8) {
                alpha = InvLerpF(0, 8, bullet.elapsed_time_);
                alpha = LerpF(0, 180, alpha);
                SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
            }

            // スプライトプロパティ取得
            const auto &prop = properties[static_cast<size_t>(bullet.type_)];
            int image_handle = g_game.asset->GetImageAsset(prop.image_id_);
            int image_height = 0;
            GetGraphSize(image_handle, nullptr, &image_height);

            // スプライト描画
            int src_x = (bullet.animation_time_ / prop.animation_interval_) * prop.image_width_; // 現在フレームから画像アセットの描画範囲を算出する
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha * 255);
            DrawRectRotaGraphF(bullet.x_, bullet.y_, src_x, 0, prop.image_width_, image_height, 1.0, 0.0, image_handle, true);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        }
    }

    // 敵に当たったとき
    void PlayerBulletManager::OnHit(PlayerBullet &bullet) {
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
}
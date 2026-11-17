#include "enemy_bullet.h"
#include <iostream>
#include <DxLib.h>
#include "asset.h"
#include "game.h"
#include "global.h"

namespace game {

    // 敵弾の描画
    void EnemyBulletManager::Draw() {
        const auto &properties = GetSpriteProperties();

        SetDrawMode(DX_DRAWMODE_BILINEAR);

        for (const auto &bullet : GetEntities()) {
            if (!bullet.is_active_) continue;

            // スポーン時のアニメーション
            float scale = 1.0f;
            float alpha = 1.0f;
            if (bullet.elapsed_time_ >= 0 && bullet.elapsed_time_ <= kEnemyBulletEnterEffectLength) {
                scale = 1.0f + InvLerpF(kEnemyBulletEnterEffectLength, 0, bullet.elapsed_time_);
                alpha = InvLerpF(0, kEnemyBulletEnterEffectLength, bullet.elapsed_time_);
            }

            // スプライトプロパティ取得
            const auto &prop = properties[static_cast<size_t>(bullet.type_)];
            int image_handle = g_game.asset->GetImageAsset(prop.image_id_);
            int image_height = 0;
            GetGraphSize(image_handle, nullptr, &image_height);

            // スプライト描画
            int src_x = (bullet.animation_time_ / prop.animation_interval_) * prop.image_width_; // 現在フレームから画像アセットの描画範囲を算出する
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha * 255);
            DrawRectRotaGraphF(bullet.x_, bullet.y_, src_x, 0, prop.image_width_, image_height, scale, GetDrawAngle(bullet), image_handle, true);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        }

        SetDrawMode(DX_DRAWMODE_NEAREST);
    }

    // 敵弾のスポーン
    EnemyBullet *EnemyBulletManager::Spawn(float x, float y, float angle, float speed, EnemyBulletSprite sprite) {
        auto *bullet = Spawn(sprite, x, y);

        if (bullet) {
            // 固有のメンバ変数を設定
            bullet->vx_ = cosf(angle) * speed;
            bullet->vy_ = sinf(angle) * speed;
            bullet->angle_ = angle;
            bullet->speed_ = speed;
        } else {
            // 敵弾が追加できなかった場合
            g_game.log->Print("EnemyBulletManager: 敵弾のスポーンに失敗しました");
        }

        return bullet;
    }

    // プレイヤーに当たったとき
    void EnemyBulletManager::OnHitPlayer(EnemyBullet &bullet) {
        bullet.is_active_ = false; // 弾を消す
    }

    // 描画角度を弾ごとに決定する関数
    float EnemyBulletManager::GetDrawAngle(const EnemyBullet &bullet) const {
        switch (bullet.type_) {
        case EnemyBulletSprite::SpinningRedRice:
            return bullet.elapsed_time_ * kEnemyBulletSpinningAnimationSpeed;
        default:
            return bullet.angle_ + (kPi / 2.0f);
        }
    }
}
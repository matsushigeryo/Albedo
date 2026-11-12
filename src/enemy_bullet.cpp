#include "enemy_bullet.h"
#include <iostream>
#include <DxLib.h>
#include "global.h"

namespace game {
    void EnemyBulletManager::Init() {
        for (auto &bullet : bullets_) {
            bullet.is_active_ = false;
        }
    }

    void EnemyBulletManager::Update() {
        for (auto &bullet : bullets_) {
            if (!bullet.is_active_) continue;

            // 座標更新
            bullet.x_ += bullet.vx_;
            bullet.y_ += bullet.vy_;

            // 画面外に出たら消去
            const float bullet_radius = 8;
            if (bullet.x_ + bullet_radius < 0 || bullet.x_ - bullet_radius > kGraphicWidth || bullet.y_ + bullet_radius < 0 || bullet.y_ - bullet_radius > kGraphicHeight) {
                bullet.is_active_ = false;
            }

            // 時間経過
            bullet.elapsed_time_++;
        }
    }

    void EnemyBulletManager::Draw() {
        SetDrawMode(DX_DRAWMODE_BILINEAR);

        int image_handle = asset.GetImageAsset(ImageID::EnemyBullet);
        float flash_add = 0.0f;

        for (const auto &bullet : bullets_) {
            if (!bullet.is_active_) continue;

            // スポーン時のアニメーション
            float scale = 1.0f;
            float alpha = 1.0f;
            if (bullet.elapsed_time_ >= 0 && bullet.elapsed_time_ <= kEnemyBulletEnterEffectLength) {
                scale = 1.0f + InvLerpF(kEnemyBulletEnterEffectLength, 0, bullet.elapsed_time_);
                alpha = InvLerpF(0, kEnemyBulletEnterEffectLength, bullet.elapsed_time_);
            }

            // 描画するソース範囲をShape及びColorから算出する
            const int src_size = 15;
            const float src_x = static_cast<float>(static_cast<int>(bullet.shape_) * src_size);
            const float src_y = static_cast<float>(static_cast<int>(bullet.color_) * src_size);

            // 描画角度を算出する
            float draw_angle = bullet.angle_ + (kPi / 2.0f);
            if (bullet.shape_ == EnemyBulletShape::SmallCircle || bullet.shape_ == EnemyBulletShape::MidiumCircle) {
                draw_angle = 0.0f;
            }

            // スプライト描画
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha * 255);
            DrawRectRotaGraphF(bullet.x_, bullet.y_, src_x, src_y, src_size, src_size, scale, draw_angle, image_handle, true);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        }

        SetDrawMode(DX_DRAWMODE_NEAREST);
    }

    bool EnemyBulletManager::AddBullet(float x, float y, float angle, float speed, EnemyBulletShape shape, EnemyBulletColor color) {
        for (auto &bullet : bullets_) { // 配列から空きを見つけて弾を追加
            if (bullet.is_active_) continue;
            bullet.is_active_ = true;
            bullet.x_ = x;
            bullet.y_ = y;
            bullet.vx_ = cosf(angle) * speed;
            bullet.vy_ = sinf(angle) * speed;
            bullet.angle_ = angle;
            bullet.speed_ = speed;
            bullet.elapsed_time_ = 0;
            bullet.shape_ = shape;
            bullet.color_ = color;
            return true;
        }

        // 弾が追加できなかった場合
        // std::cerr << "敵弾の追加に失敗しました。" << std::endl;
        return false;
    }

    // プレイヤーに当たったとき
    void EnemyBulletManager::OnHitPlayer(EnemyBullet &bullet) {
        bullet.is_active_ = false; // 弾を消す
    }

    // 弾の配列を取得する
    std::array<EnemyBullet, EnemyBulletManager::kMaxNum> &EnemyBulletManager::GetBullets() {
        return bullets_;
    }

    // アクティブな弾の数を取得する
    size_t EnemyBulletManager::GetActiveBulletCount() const {
        size_t count = 0;
        for (const auto &bullet : bullets_) {
            if (bullet.is_active_) {
                count++;
            }
        }
        return count;
    }
}
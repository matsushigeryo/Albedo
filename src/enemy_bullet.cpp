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
            const float bullet_radius = 4;
			if (bullet.x_ + bullet_radius < 0 || bullet.x_ - bullet_radius > kGraphicWidth || bullet.y_ + bullet_radius < 0 || bullet.y_ - bullet_radius > kGraphicHeight) {
				bullet.is_active_ = false;
			}

            // 時間経過
            bullet.elapsed_time_++;
		}
    }

    void EnemyBulletManager::Draw() {
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

            // スプライト描画
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha * 255);
            DrawRotaGraphF(bullet.x_, bullet.y_, scale, 0.0, image_handle, true);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        }
    }

    bool EnemyBulletManager::AddBullet(float x, float y, float angle, float speed) {
        for (auto &bullet : bullets_) { // 配列から空きを見つけて弾を追加
            if (bullet.is_active_) continue;
            bullet.is_active_ = true;
            bullet.x_ = x;
            bullet.y_ = y;
            bullet.vx_ = sinf(angle) * speed;
            bullet.vy_ = cosf(angle) * speed;
            bullet.elapsed_time_ = 0;
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
}
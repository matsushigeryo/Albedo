#include "player_bullet_manager.h"
#include <iostream>
#include <DxLib.h>
#include "global.h"

namespace game {
    PlayerBulletManager::PlayerBulletManager() {
        image_handle_mainshot_ = asset.GetImageAsset(ImageID::PlayerBulletMainShot);
        image_handle_subshot_ = asset.GetImageAsset(ImageID::PlayerBulletSubShot);
    }

    void PlayerBulletManager::Init() {
        for (auto &bullet : bullets_) {
            bullet.is_active_ = false;
        }
    }

    void PlayerBulletManager::Update() {
        for (auto &bullet : bullets_) {
            if (bullet.is_active_) {
                bullet.y_ -= kBulletSpeed; // 座標を更新
                bullet.elapsed_time_++;    // 経過時間を更新
                if (bullet.y_ < -32) {     // 画面外に出た弾を削除
                    bullet.is_active_ = false;
                }
            }
        }
    }

    void PlayerBulletManager::Draw() {
        int image_handle = -1;
        float alpha = 1.0f;

        for (const auto &bullet : bullets_) {
            if (bullet.is_active_) {
                // 描画する画像ハンドルを決定する
                switch (bullet.type_) {
                case BulletType::MainShot: image_handle = image_handle_mainshot_; break;
                case BulletType::SubShot: image_handle = image_handle_subshot_; break;
                }

                // 射出直後から時間経過でフェードインする
                alpha = InvLerpF(0, 8, bullet.elapsed_time_);
                if (alpha < 0.0f) alpha = 0.0f;
                if (alpha > 1.0f) alpha = 1.0f;
                alpha = LerpF(0, 180, alpha);
                SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

                // 描画
                DrawRotaGraphF(bullet.x_, bullet.y_, 1.0, 0.0, image_handle, true);
            }
        }

        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // 合成モードを元に戻す
    }

    bool PlayerBulletManager::Shot(BulletType type, float x, float y) {
        for (auto &bullet : bullets_) {
            if (!bullet.is_active_) { // 配列から空きを見つけて弾を追加
                bullet.is_active_ = true;
                bullet.type_ = type;
                bullet.x_ = x;
                bullet.y_ = y;
                bullet.elapsed_time_ = 0;
                return true;
            }
        }

        // 弾が追加できなかった場合
        std::cerr << "プレイヤー弾の追加に失敗しました。" << std::endl;
        return false;
    }

    std::array<Bullet, PlayerBulletManager::kMaxNum> &PlayerBulletManager::GetBullets() {
        return bullets_;
    }
}
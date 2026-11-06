#include "player.h"
#include <DxLib.h>
#include "global.h"

namespace game {
    Player::Player() {
        image_handle_ = asset.GetImageAsset(ImageID::Player);
        image_handle_option_ = asset.GetImageAsset(ImageID::PlayerOption);
    }

    void Player::Init() {
        x_ = kGraphicWidth / 2;
        y_ = kGraphicHeight - 32;
        dead_timer_ = kInvincibleTime + 1;
        shot_timer_ = 0;
        options_formation_anim_progress_ = 0.0f;
    }

    void Player::Update() {
        // 入力フラグを取得
        const bool input_right = input.OnHeldKeyboard(KEY_INPUT_RIGHT);
        const bool input_left = input.OnHeldKeyboard(KEY_INPUT_LEFT);
        const bool input_down = input.OnHeldKeyboard(KEY_INPUT_DOWN);
        const bool input_up = input.OnHeldKeyboard(KEY_INPUT_UP);
        const bool input_shot = input.OnHeldKeyboard(KEY_INPUT_Z);
        const bool input_slow = input.OnHeldKeyboard(KEY_INPUT_LSHIFT);

        // 移動速度を計算する
        float move_speed = input_slow ? kBaseSpeedSlow : kBaseSpeed;
        if ((input_right ^ input_left) && (input_down ^ input_up)) {
            move_speed /= kSqrt2;
        }

        // 座標を更新する
        if (input_right) x_ += move_speed;
        if (input_left) x_ -= move_speed;
        if (input_down) y_ += move_speed;
        if (input_up) y_ -= move_speed;

        // 画面端で移動を制限する
        if (x_ < kScreenBoundaryCollisionRadius) x_ = kScreenBoundaryCollisionRadius;
        if (y_ < kScreenBoundaryCollisionRadius) y_ = kScreenBoundaryCollisionRadius;
        if (x_ > kGraphicWidth - kScreenBoundaryCollisionRadius) x_ = kGraphicWidth - kScreenBoundaryCollisionRadius;
        if (y_ > kGraphicHeight - kScreenBoundaryCollisionRadius) y_ = kGraphicHeight - kScreenBoundaryCollisionRadius;

        // オプションの座標を更新
        int option_index = 0;
        const float orbit_radius = LerpF(20, 12, EaseOutQuad(options_formation_anim_progress_));
        for (auto &option : options_) {
            option.offset_x_ = std::cosf((time / 60.0f + option_index / static_cast<float>(options_.size())) * kPi * 2.0f) * orbit_radius;
            option.offset_y_ = std::sinf((time / 60.0f + option_index / static_cast<float>(options_.size())) * kPi * 2.0f) * orbit_radius;
            option_index++;
        }

        // オプションの配置切り替えによる移動アニメーションの更新
        if (input_slow) {
            options_formation_anim_progress_ += kOptionsFormationAnimProgressPerFrame;
            if (options_formation_anim_progress_ > 1.0f) options_formation_anim_progress_ = 1.0f;
        } else {
            options_formation_anim_progress_ -= kOptionsFormationAnimProgressPerFrame;
            if (options_formation_anim_progress_ < 0.0f) options_formation_anim_progress_ = 0.0f;
        }

        // ショットを放つ
        if (input_shot) {
            if (shot_timer_ % kShotIntervalFrame == 0) {
                player_bullet_manager_ptr->Shot(PlayerBulletType::MainShot, x_, y_);
                for (const auto &option : options_) {
                    player_bullet_manager_ptr->Shot(PlayerBulletType::SubShot, x_ + option.offset_x_, y_ + option.offset_y_);
                }
            }
            shot_timer_++;
        } else {
            shot_timer_ = 0;
        }

        // タイマー更新
        dead_timer_++;
    }

    void Player::Draw() {
        if (IsInvincible()) { // 無敵時間中は点滅させる
            if (dead_timer_ / 2 % 2 == 0) {
                SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
            }
        }

        DrawRotaGraphF(x_, y_, 1.0, 0.0, image_handle_, true); // プレイヤー本体の描画
        for (const auto &option : options_) {                  // オプションの描画
            DrawRotaGraphF(x_ + option.offset_x_, y_ + option.offset_y_, 1.0, 0.0, image_handle_option_, true);
        }

        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0); // 合成モードを元に戻す
    }

    // ダメージを受けたとき
    void Player::OnDamage() {
        if (!IsInvincible()) {
            dead_timer_ = 0;
        }
    }

    float Player::GetX() const {
        return x_;
    }

    float Player::GetY() const {
        return y_;
    }

    bool Player::IsInvincible() const {
        return dead_timer_ < kInvincibleTime;
    }
}
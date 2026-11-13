#include "player_bomb.h"
#include <DxLib.h>
#include "global.h"

namespace game {
    PlayerBomb::PlayerBomb(int init_stock)
        : init_stock_{init_stock}, stock_{init_stock} {
    }

    void PlayerBomb::Update() {
        // タイマー更新
        left_timer_--;
        if (left_timer_ < 0) left_timer_ = 0;

        // ボム発動
        if (left_timer_ == 0 && input.OnPressedKeyboard(KEY_INPUT_X)) {
            left_timer_ = kDurationTime;
        }
    }

    void PlayerBomb::Draw() {
        if (left_timer_ > 0) {
            const float norm = InvLerpF(kDurationTime, 0, left_timer_);
            SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
            DrawCircle(kGraphicWidth / 2.0f, kGraphicHeight / 2.0f, static_cast<float>(kGraphicWidth / 2.0f) * norm, GetColor(255, 255, 255), true);
            SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        }
    }
}
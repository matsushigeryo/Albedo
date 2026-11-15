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
            const int timer = kDurationTime - left_timer_;
            const float gw = static_cast<float>(kGraphicWidth);
            const float gh = static_cast<float>(kGraphicHeight);

            // 背景の描画
            SetDrawBlendMode(DX_BLENDMODE_MULA, InvLerpF(0, 60, timer) * 256);
            DrawBox(0, 0, gw, gh, GetColor(80, 250, 230), true);
        }

        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }
}
#include "player_bomb.h"
#include <DxLib.h>
#include "global.h"

namespace game {
    PlayerBomb::PlayerBomb(int init_stock)
        : init_stock_{init_stock}, stock_{init_stock} {
    }

    void PlayerBomb::Update() {
        // É^ÉCÉ}Å[çXêV
        left_timer_--;
        if (left_timer_ < 0) left_timer_ = 0;

        // É{ÉÄî≠ìÆ
        if (left_timer_ == 0 && input.OnPressedKeyboard(KEY_INPUT_X)) {
            left_timer_ = kDurationTime;
        }
    }

    void PlayerBomb::Draw() {
        if (left_timer_ > 0) {
            const int timer = kDurationTime - left_timer_;
            const float gw = static_cast<float>(kGraphicWidth);
            const float gh = static_cast<float>(kGraphicHeight);

            // îwåiÇÃï`âÊ
            SetDrawBlendMode(DX_BLENDMODE_MULA, InvLerpF(0, 60, timer) * 256);
            DrawBox(0, 0, gw, gh, GetColor(80, 250, 230), true);

            // ìGÉ^Å[ÉQÉbÉeÉBÉìÉOògê¸ÇÃï`âÊ
            SetDrawBlendMode(DX_BLENDMODE_ADD, InvLerpF(0, 60, timer) * 256);
            for (Enemy &enemy : enemy_manager_ptr->GetEnemies()) {
                if (!enemy.is_active_) continue;
                const float ex = enemy.x_;
                const float ey = enemy.y_;
                DrawCircle(ex, ey, 32, GetColor(80, 250, 230), false, 1);
            }
        }
    }
}
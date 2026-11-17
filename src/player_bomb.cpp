#include "player_bomb.h"
#include <DxLib.h>
#include "game.h"
#include "input.h"

namespace game {
    PlayerBomb::PlayerBomb(int init_stock)
        : init_stock_{init_stock}, stock_{init_stock} {
    }

    void PlayerBomb::Update() {
        // タイマー更新
        left_timer_--;
        if (left_timer_ < 0) left_timer_ = 0;

        // ボム発動
        if (left_timer_ == 0 && g_game.input->OnPressedKeyboard(KEY_INPUT_X)) {
            left_timer_ = kDurationTime;
        }
    }

    void PlayerBomb::Draw() {
        if (left_timer_ > 0) {
            const int timer = kDurationTime - left_timer_;
        }
    }
}
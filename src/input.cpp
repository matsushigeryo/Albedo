#include "input.h"
#include <DxLib.h>

namespace game {
    void Input::Init() {
        memset(keyboard_current_state_, 0, sizeof(keyboard_current_state_));
        memset(keyboard_previous_state_, 0, sizeof(keyboard_previous_state_));
    }

    void Input::Update() {
        // キーボードの入力状態を更新する
        memcpy(keyboard_previous_state_, keyboard_current_state_, sizeof(keyboard_previous_state_));
        GetHitKeyStateAll(keyboard_current_state_);
    }

    // キーボードのキーが押されているか？
    bool Input::OnHeldKeyboard(const unsigned char key_code) const {
        return keyboard_current_state_[key_code] == 1;
    }

    // キーボードのキーが押された瞬間か？
    bool Input::OnPressedKeyboard(const unsigned char key_code) const {
        return keyboard_current_state_[key_code] == 1 && keyboard_previous_state_[key_code] != 1;
    }

    // キーボードのキーが話された瞬間か？
    bool Input::OnReleasedKeyboard(const unsigned char key_code) const {
        return keyboard_current_state_[key_code] != 1 && keyboard_previous_state_[key_code] == 1;
    }
}
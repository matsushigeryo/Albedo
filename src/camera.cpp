#include "camera.h"
#include <DxLib.h>
#include "global.h"

namespace game {
    void Camera::Init(float x, float y) {
        x_ = x;
        y_ = y;
    }

    void Camera::Update() {
        y_ -= 0.1f;
        if (y_ < 0.0f) y_ = 0.0f;
    }

    float Camera::GetX() const {
        return x_;
    }

    float Camera::GetY() const {
        return y_;
    }
}
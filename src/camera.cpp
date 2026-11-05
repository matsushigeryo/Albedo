#include "camera.h"
#include <DxLib.h>
#include "global.h"

namespace game {
    void Camera::Init() {
        x_ = 0.0f;
        y_ = tilemap_ptr->GetMapHeight() - kGraphicHeight / TileMap::kTileDrawHeight;
    }

    void Camera::Update() {
        y_ -= 0.1f;
    }

    float Camera::GetX() const {
        return x_;
    }

    float Camera::GetY() const {
        return y_;
    }
}
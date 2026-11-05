#ifndef ENEMY_H_
#define ENEMY_H_

#include <array>
#include "asset.h"

namespace game {
    enum class EnemySprite {
        Kuro,
        ENUM_END
    };
    constexpr size_t kEnemySpriteSize = static_cast<size_t>(EnemySprite::ENUM_END);

    constexpr std::array<ImageID, kEnemySpriteSize> kEnemySpriteImageIDs = {
        ImageID::EnemyKuro};

    struct Enemy {
        bool is_active_ = false;
        EnemySprite sprite_;
        float x_ = 0.0f;
        float y_ = 0.0f;
        int elapsed_time_ = 0;
        int on_damage_timer_ = 0;
    };
}

#endif
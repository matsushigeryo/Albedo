#ifndef PLAYER_BULLET_H_
#define PLAYER_BULLET_H_

namespace game {
    enum class BulletType {
        MainShot,
        SubShot,
    };

    struct Bullet {
        bool is_active_ = false;
        BulletType type_;
        float x_ = 0.0f;
        float y_ = 0.0f;
        int elapsed_time_ = 0;
    };
}

#endif
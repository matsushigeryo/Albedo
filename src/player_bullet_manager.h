#ifndef PLAYER_BULLETS_H_
#define PLAYER_BULLETS_H_

#include <array>
#include "player_bullet.h"

namespace game {
    class PlayerBulletManager {
      public:
        static constexpr size_t kMaxNum = 64;
        static constexpr float kBulletSpeed = 8.0f;

      public:
        PlayerBulletManager();
        ~PlayerBulletManager() = default;
        void Init();
        void Update();
        void Draw();
        bool Shot(BulletType type, float x, float y);
        std::array<Bullet, kMaxNum> &GetBullets();

      private:
        std::array<Bullet, kMaxNum> bullets_{};
        int image_handle_mainshot_ = -1;
        int image_handle_subshot_ = -1;
    };
}

#endif
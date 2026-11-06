#ifndef PLAYER_H_
#define PLAYER_H_

#include <array>

namespace game {
    class Player {
      public:
        static constexpr float kBaseSpeed = 3.0f;
        static constexpr float kBaseSpeedSlow = 1.5f;
        static constexpr int kInvincibleTime = 90;
        static constexpr int kShotIntervalFrame = 5;
        static constexpr float kScreenBoundaryCollisionRadius = 7;
        static constexpr float kOptionsFormationAnimProgressPerFrame = 0.2f;

        struct Option {
            float offset_x_ = 0.0f;
            float offset_y_ = 0.0f;
        };

      public:
        Player();
        ~Player() = default;
        void Init();
        void Update();
        void Draw();
        void OnDamage();
        float GetX() const;
        float GetY() const;
        bool IsInvincible() const;

      private:
        int image_handle_ = -1;
        int image_handle_option_ = -1;
        float x_ = 0.0f;
        float y_ = 0.0f;
        int dead_timer_ = kInvincibleTime + 1;
        int shot_timer_ = 0;
        std::array<Option, 3> options_{};
        float options_formation_anim_progress_ = 0.0f;
    };
}

#endif
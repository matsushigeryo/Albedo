#ifndef PLAYER_BULLET_H_
#define PLAYER_BULLET_H_

#include <array>

namespace game {
    enum class PlayerBulletType {
        MainShot,
        SubShot,
    };

    struct PlayerBullet {
        bool is_active_ = false;
        PlayerBulletType type_;
        float x_ = 0.0f;
        float y_ = 0.0f;
        int elapsed_time_ = 0;
    };

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
        bool Shot(PlayerBulletType type, float x, float y);
        void OnHitEnemy(PlayerBullet &bullet);
        std::array<PlayerBullet, kMaxNum> &GetBullets();

      private:
        std::array<PlayerBullet, kMaxNum> bullets_{};
        int image_handle_mainshot_ = -1;
        int image_handle_subshot_ = -1;
    };
}

#endif
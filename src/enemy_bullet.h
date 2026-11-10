#ifndef ENEMY_BULLET_H_
#define ENEMY_BULLET_H_

#include <array>

namespace game {
    struct EnemyBullet {
        bool is_active_ = false;
        float x_ = 0.0f;
        float y_ = 0.0f;
        float vx_ = 0.0f;
        float vy_ = 0.0f;
        int elapsed_time_ = 0;
    };

    constexpr float kEnemyBulletEnterEffectLength = 10;

    class EnemyBulletManager {
      public:
        static constexpr int kMaxNum = 512;

      public:
        EnemyBulletManager() = default;
        ~EnemyBulletManager() = default;
        void Init();
        void Update();
        void Draw();
        bool AddBullet(float x, float y, float angle, float speed);
        void OnHitPlayer(EnemyBullet &bullet);
        std::array<EnemyBullet, kMaxNum> &GetBullets();

      private:
        std::array<EnemyBullet, kMaxNum> bullets_{};
    };
}

#endif
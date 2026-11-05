#ifndef ENEMY_MANAGER_H_
#define ENEMY_MANAGER_H_

#include <array>
#include "enemy.h"

namespace game {
    class EnemyManager {
      public:
        static constexpr size_t kMaxNum = 64;

      public:
        EnemyManager() = default;
        ~EnemyManager() = default;
        void Init();
        void Update();
        void Draw();
        bool Spawn(EnemySprite sprite, float x, float y);
        void OnDamage(Enemy &enemy);
        std::array<Enemy, kMaxNum> GetEnemies();

      private:
        std::array<Enemy, kMaxNum> enemies_{};
    };
}

#endif
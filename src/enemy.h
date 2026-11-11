#ifndef ENEMY_H_
#define ENEMY_H_

#include <array>
#include "asset.h"
#include "enemy_script.h"

namespace game {

    // 敵のスプライト種別
    enum class EnemySprite {
        Kuro,
        ENUM_END
    };

    constexpr size_t kEnemySpriteSize = static_cast<size_t>(EnemySprite::ENUM_END);

    // 敵のスプライト種別に対応する画像ID
    constexpr std::array<ImageID, kEnemySpriteSize> kEnemySpriteImageIDs = {
        ImageID::EnemyKuro,
    };

    constexpr int kEnemyOnDamageFlashEffectLength = 4;

    struct Enemy {
        bool is_active_ = false;
        EnemySprite sprite_;
        float x_ = 0.0f;
        float y_ = 0.0f;
        int elapsed_time_ = 0;
        int on_damage_timer_ = 0;
        int life_ = 0;
        std::unique_ptr<enemy_script::EnemyScriptRunner> script_runner_{nullptr};
    };

    class EnemyManager {
      public:
        static constexpr size_t kMaxNum = 64;

      public:
        EnemyManager() = default;
        ~EnemyManager() = default;
        void Init();
        void Update();
        void Draw();
        bool Spawn(EnemySprite sprite, float x, float y, std::string script_id = "");
        void Die(Enemy &enemy);
        void OnHitPlayerBullet(Enemy &enemy);
        std::array<Enemy, kMaxNum> &GetEnemies();
        size_t GetActiveEnemyCount() const;

      private:
        std::array<Enemy, kMaxNum> enemies_{};
    };
}

#endif
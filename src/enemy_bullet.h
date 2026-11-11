#ifndef ENEMY_BULLET_H_
#define ENEMY_BULLET_H_

#include <array>
#include <string>
#include <unordered_map>

namespace game {
    enum class EnemyBulletShape {
        SmallCircle,
        Spike,
        Rice,
        MidiumCircle,
    };

    enum class EnemyBulletColor {
        White,
        Red,
        Blue,
    };

    // 文字列から弾の形状を取得する関数
    static EnemyBulletShape EnemyBulletShapeFromString(const std::string &str) {
        static const std::unordered_map<std::string, EnemyBulletShape> kEnemyBulletShapeMap = {
            {"SmallCircle", EnemyBulletShape::SmallCircle},
            {"Spike", EnemyBulletShape::Spike},
            {"Rice", EnemyBulletShape::Rice},
            {"MidiumCircle", EnemyBulletShape::MidiumCircle},
        };
        if (kEnemyBulletShapeMap.find(str) != kEnemyBulletShapeMap.end()) {
            return kEnemyBulletShapeMap.at(str);
        } else {
            return EnemyBulletShape::SmallCircle; // 見つからなかった場合はデフォルト値を返す
        }
    }

    // 文字列から弾の色を取得する関数
    static EnemyBulletColor EnemyBulletColorFromString(const std::string &str) {
        static const std::unordered_map<std::string, EnemyBulletColor> kEnemyBulletColorMap = {
            {"White", EnemyBulletColor::White},
            {"Red", EnemyBulletColor::Red},
            {"Blue", EnemyBulletColor::Blue},
        };
        if (kEnemyBulletColorMap.find(str) != kEnemyBulletColorMap.end()) {
            return kEnemyBulletColorMap.at(str);
        } else {
            return EnemyBulletColor::Red; // 見つからなかった場合はデフォルト値を返す
        }
    }

    struct EnemyBullet {
        bool is_active_ = false;
        float x_ = 0.0f;
        float y_ = 0.0f;
        float vx_ = 0.0f;
        float vy_ = 0.0f;
        float angle_ = 0.0f;
        float speed_ = 0.0f;
        int elapsed_time_ = 0;
        EnemyBulletShape shape_;
        EnemyBulletColor color_;
    };

    constexpr float kEnemyBulletEnterEffectLength = 10;

    class EnemyBulletManager {
      public:
        static constexpr int kMaxNum = 1024;

      public:
        EnemyBulletManager() = default;
        ~EnemyBulletManager() = default;
        void Init();
        void Update();
        void Draw();
        bool AddBullet(float x, float y, float angle, float speed, EnemyBulletShape shape, EnemyBulletColor color);
        void OnHitPlayer(EnemyBullet &bullet);
        std::array<EnemyBullet, kMaxNum> &GetBullets();
        size_t GetActiveBulletCount() const;

      private:
        std::array<EnemyBullet, kMaxNum> bullets_{};
    };
}

#endif
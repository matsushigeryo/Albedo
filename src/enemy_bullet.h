#ifndef ENEMY_BULLET_H_
#define ENEMY_BULLET_H_

#include <array>
#include <string>
#include <unordered_map>
#include "asset.h"
#include "entity_manager.h"

namespace game {

    constexpr float kEnemyBulletEnterEffectLength = 10;        // 発射時のアニメーション効果の長さ
    constexpr float kEnemyBulletSpinningAnimationSpeed = 0.1f; // 回転アニメーションの速度(Radian/F)

    // 敵弾のスプライト種別
    enum class EnemyBulletSprite {
        WhiteSmallCircle,
        SpinningRedRice,
        GreenNeedle,
        ENUM_END
    };

    // 文字列から敵弾のスプライト種別を取得する関数
    static EnemyBulletSprite EnemyBulletSpriteFromString(const std::string &str) {
        static const std::unordered_map<std::string, EnemyBulletSprite> kEnemyBulletSpriteMap = {
            {"WhiteSmallCircle", EnemyBulletSprite::WhiteSmallCircle},
            {"SpinningRedRice", EnemyBulletSprite::SpinningRedRice},
            {"GreenNeedle", EnemyBulletSprite::GreenNeedle},
        };
        if (kEnemyBulletSpriteMap.find(str) != kEnemyBulletSpriteMap.end()) {
            return kEnemyBulletSpriteMap.at(str);
        } else {
            return EnemyBulletSprite::WhiteSmallCircle; // 見つからなかった場合はデフォルト値を返す
        }
    }

    // 敵弾の構造体
    struct EnemyBullet : public EntityBase<EnemyBulletSprite> {
        float angle_ = 0.0f;
        float speed_ = 0.0f;
    };

    // 敵弾管理のクラス
    class EnemyBulletManager : public EntityManager<EnemyBullet, EnemyBulletSprite, 1024> {
      public:
        using EntityManager::Spawn;

        void Draw() override;
        EnemyBullet *Spawn(float x, float y, float angle, float speed, EnemyBulletSprite sprite);
        void OnHitPlayer(EnemyBullet &bullet);

      protected:
        const std::array<SpriteProperty, kTypeSize> &GetSpriteProperties() const override {
            static const std::array<SpriteProperty, kTypeSize> kProperties = {{
                {ImageID::EnemyBullet_WhiteSmallCircle, 15, 1, 1},
                {ImageID::EnemyBullet_SpinningRedRice, 15, 1, 1},
                {ImageID::EnemyBullet_GreenNeedle, 15, 1, 1},
            }};
            return kProperties;
        }

      private:
        float GetDrawAngle(const EnemyBullet &bullet) const;
    };
}

#endif
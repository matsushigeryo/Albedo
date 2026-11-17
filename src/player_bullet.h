#ifndef PLAYER_BULLET_H_
#define PLAYER_BULLET_H_

#include <array>
#include "entity_manager.h"

namespace game {

    constexpr float kBulletSpeed = 8.0f;

    // プレイヤー弾のスプライト種別
    enum class PlayerBulletSprite {
        MainShot,
        SubShot,
        ENUM_END
    };

    // プレイヤー弾の構造体
    struct PlayerBullet : public EntityBase<PlayerBulletSprite> {};

    // プレイヤー弾管理のクラス
    class PlayerBulletManager : public EntityManager<PlayerBullet, PlayerBulletSprite, 1024> {
      public:
        void Draw() override;
        void OnHit(PlayerBullet &bullet);

      protected:
        const std::array<SpriteProperty, kTypeSize> &GetSpriteProperties() const override {
            static const std::array<SpriteProperty, kTypeSize> kProperties = {{
                {ImageID::PlayerBulletMainShot, 15, 1, 1}, // MainShot
                {ImageID::PlayerBulletSubShot, 15, 1, 1},  // SubShot
            }};
            return kProperties;
        }

        void UniqueSpawn(PlayerBullet &bullet) override;
        void UniqueUpdate(PlayerBullet &bullet) override;
    };
}

#endif
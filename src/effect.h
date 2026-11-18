#ifndef EFFECT_H_
#define EFFECT_H_

#include <array>
#include "entity_manager.h"

namespace game {

    // エフェクトの種別
    enum class EffectType {
        PlayerBulletSparkle,
        EnemyDead,
        ENUM_END
    };

    // エフェクトの構造体
    struct Effect : public EntityBase<EffectType> {};

    // エフェクト管理のクラス
    class EffectManager : public EntityManager<Effect, EffectType, 128> {
      protected:
        const std::array<SpriteProperty, kTypeSize> &GetSpriteProperties() const override {
            static const std::array<SpriteProperty, kTypeSize> kProperties = {{
                {ImageID::EffectWhiteSparkle1, 15, 4, 4, AnimationMode::Destroy},  // PlayerBulletSparkle
                {ImageID::EffectWhiteSparkle2, 15, 11, 1, AnimationMode::Destroy}, // EnemyDead
            }};
            return kProperties;
        }

        void UniqueSpawn(Effect &effect) override;
    };
}

#endif
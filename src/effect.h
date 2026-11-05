#ifndef EFFECT_H_
#define EFFECT_H_

#include <array>
#include "asset.h"

namespace game {

    // エフェクトの種別
    enum class EffectType {
        PlayerBulletSparkle,
        EnemyDead,
        ENUM_END
    };

    constexpr size_t kEffectTypeSize = static_cast<size_t>(EffectType::ENUM_END);

    // スプライトプロパティの構造体
    struct EffectSpriteProperty {
        EffectSpriteProperty(ImageID image_id, int frame_count, int frame_width, int frame_animation_interval, bool is_loop)
            : image_id_{image_id}, frame_count_{frame_count}, frame_width_{frame_width}, frame_animation_interval_{frame_animation_interval}, is_loop_{is_loop} {
        }

        const ImageID image_id_;
        const int frame_count_;
        const int frame_width_;
        const int frame_animation_interval_;
        const bool is_loop_;
    };

    // エフェクトの種別に対応するスプライトプロパティ
    const std::array<EffectSpriteProperty, kEffectTypeSize> kEffectSpriteProperties = {
        // EffectSpriteProperty(画像ID, フレーム総数, フレーム横幅, フレーム送り間隔, ループフラグ)
        EffectSpriteProperty(ImageID::EffectWhiteSparkle1, 4, 15, 4, false),
        EffectSpriteProperty(ImageID::EffectWhiteSparkle2, 11, 15, 1, false),
    };

    struct Effect {
        bool is_active_ = false;
        EffectType type_;
        float x_ = 0.0f;
        float y_ = 0.0f;
        float vx_ = 0.0f;
        float vy_ = 0.0f;
        int elapsed_time_ = 0;
    };

    class EffectManager {
      public:
        static constexpr size_t kMaxNum = 64;

      public:
        EffectManager() = default;
        ~EffectManager() = default;
        void Init();
        void Update();
        void Draw();
        bool Spawn(EffectType type, float x, float y);
        void UniqueSpawn(Effect &effect);
        void UniqueUpdate(Effect &effect);

      private:
        std::array<Effect, kMaxNum> effects_{};
    };
}

#endif
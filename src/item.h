#ifndef ITEM_H_
#define ITEM_H_

#include <array>
#include "asset.h"
#include "entity_manager.h"

namespace game {

    // アイテムの種別
    enum class ItemType {
        Coin,
        ENUM_END
    };

    // アイテムの構造体
    struct Item : public EntityBase<ItemType> {
        bool on_hit_player_flag_ = false;
        int on_hit_player_timer_ = 0;
        float on_hit_player_position_y_ = 0;
        bool player_sticked_ = false;
    };

    // アイテムの管理クラス
    class ItemManager : public EntityManager<Item, ItemType, 128> {

        static constexpr float kStartVelocity = -1.5f;
        static constexpr float kAddVelocity = 0.025f;
        static constexpr float kMaxVelocity = 1.0f;
        static constexpr int kOnHitPlayerAnimDuration = 30;
        static constexpr int kPlayerPickUpAnimationDistanceY = -16;
        static constexpr int kPlayerPickUpAnimationFlashInterval = 3;
        static constexpr int kPlayerStickEffectRadius = 42;
        static constexpr float kPlayerStickEffectSpeed = 3.5f;

      public:
        void OnHitPlayer(Item &item);

      protected:
        const std::array<SpriteProperty, kTypeSize> &GetSpriteProperties() const override {
            static const std::array<SpriteProperty, kTypeSize> kProperties = {{
                {ImageID::ItemCoin, 15, 4, 4, AnimationMode::Loop}, // Coin
            }};
            return kProperties;
        }

        void UniqueSpawn(Item &item) override;
        void UniqueUpdate(Item &item) override;
        void UniqueDraw(Item &item, const SpriteProperty &prop, int image_handle, int src_x, int image_height) override;

      private:
        void UpdateGravity(Item &item);
    };
}

#endif
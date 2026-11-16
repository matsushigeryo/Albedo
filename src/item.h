#ifndef ITEM_H_
#define ITEM_H_

#include <array>
#include "asset.h"

namespace game {

    // アイテムの種別
    enum class ItemType {
        Coin,
        ENUM_END
    };

    constexpr size_t kItemTypeSize = static_cast<size_t>(ItemType::ENUM_END);

    // スプライトプロパティの構造体
    struct ItemSpriteProperty {
        ItemSpriteProperty(ImageID image_id, int frame_count, int frame_width, int frame_animation_interval)
            : image_id_{image_id}, frame_count_{frame_count}, frame_width_{frame_width}, frame_animation_interval_{frame_animation_interval} {
        }

        const ImageID image_id_;
        const int frame_count_;
        const int frame_width_;
        const int frame_animation_interval_;
    };

    // アイテムの種別に対応するスプライトプロパティ
    const std::array<ItemSpriteProperty, kItemTypeSize> kItemSpriteProperties = {
        ItemSpriteProperty(ImageID::ItemCoin, 4, 15, 4),
    };

    struct Item {
        bool is_active_ = false;
        ItemType type_;
        float x_ = 0.0f;
        float y_ = 0.0f;
        float vx_ = 0.0f;
        float vy_ = 0.0f;
        int elapsed_time_ = 0;
    };

    class ItemManager {
      public:
        static constexpr size_t kMaxNum = 64;

      public:
        ItemManager() = default;
        ~ItemManager() = default;
        void Init();
        void Update();
        void Draw();
        bool Spawn(ItemType type, float x, float y);
        void UniqueSpawn(Item &item);
        void UniqueUpdate(Item &item);
        std::array<Item, kMaxNum> &GetItems();

      private:
        std::array<Item, kMaxNum> items_{};
    };
}

#endif
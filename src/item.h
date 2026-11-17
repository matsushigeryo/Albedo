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
    struct Item : public EntityBase<ItemType> {};

    // アイテムの管理クラス
    class ItemManager : public EntityManager<Item, ItemType, 128> {
      protected:
        const std::array<SpriteProperty, kTypeSize> &GetSpriteProperties() const override {
            static const std::array<SpriteProperty, kTypeSize> kProperties = {{
                {ImageID::ItemCoin, 15, 4, 4, AnimationMode::Loop}, // Coin
            }};
            return kProperties;
        }
    };
}

#endif
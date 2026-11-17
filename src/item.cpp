#include "item.h"
#include <DxLib.h>
#include "global.h"

namespace game {
    void ItemManager::Init() {
        for (auto &item : items_) {
            item.is_active_ = false;
        }
    }

    void ItemManager::Update() {
        for (auto &item : items_) {
            if (item.is_active_) {

                // エフェクト種別ごとに固有の更新をする
                UniqueUpdate(item);

                // 座標更新
                item.x_ += item.vx_;
                item.y_ += item.vy_;

                // 時間経過
                item.elapsed_time_++;

                // アニメーションのループ処理
                const ItemSpriteProperty &property = kItemSpriteProperties[static_cast<size_t>(item.type_)];
                if (item.elapsed_time_ / property.frame_animation_interval_ >= property.frame_count_) {
                    item.elapsed_time_ = 0;
                }
            }
        }
    }

    void ItemManager::Draw() {
        int image_handle = -1;
        int image_height = 0;
        int src_x = 0;

        for (const auto &item : items_) {
            if (item.is_active_) {
                const ItemSpriteProperty &property = kItemSpriteProperties[static_cast<size_t>(item.type_)];
                image_handle = asset.GetImageAsset(property.image_id_);                                                            // スプライトの画像ハンドル
                GetGraphSize(image_handle, nullptr, &image_height);                                                                // 画像の高さを取得
                src_x = (item.elapsed_time_ / property.frame_animation_interval_) * property.frame_width_;                         // 現在のアニメーションフレームより、元画像から切り出すオフセットを決定
                DrawRectRotaGraphF(item.x_, item.y_, src_x, 0, property.frame_width_, image_height, 1.0, 0.0, image_handle, true); // スプライト描画
            }
        }
    }

    bool ItemManager::Spawn(ItemType type, float x, float y) {
        for (auto &item : items_) {
            if (!item.is_active_) { // 配列から空きを見つけてアイテムを追加
                item.is_active_ = true;
                item.type_ = type;
                item.x_ = x;
                item.y_ = y;
                item.vx_ = 0.0f;
                item.vy_ = 0.0f;
                item.elapsed_time_ = 0;
                UniqueSpawn(item);
                return true;
            }
        }
        return false;
    }

    // アイテム種別ごとの固有の初期化をする
    void ItemManager::UniqueSpawn(Item &item) {
        switch (item.type_) {
        default:
            break;
        }
    }

    // アイテム種別ごとの固有の更新をする
    void ItemManager::UniqueUpdate(Item &item) {
        switch (item.type_) {
        default:
            break;
        }
    }

    // アイテムの配列を取得する
    std::array<Item, ItemManager::kMaxNum> &ItemManager::GetItems() {
        return items_;
    }
}
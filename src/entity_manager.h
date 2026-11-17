#ifndef ENTITY_MANAGER_H_
#define ENTITY_MANAGER_H_

#include <array>
#include <DxLib.h>
#include "asset.h"
#include "global.h"

namespace game {

    // スプライトのアニメーションモード
    enum class AnimationMode {
        Loop,
        Destroy,
        Hold
    };

    // スプライトの描画情報を示す構造体
    struct SpriteProperty {
        ImageID image_id_;
        int image_width_;
        int animation_num_;
        int animation_interval_;
        AnimationMode animation_mode_ = AnimationMode::Loop;
    };

    // 全エンティティ共通の基底構造体
    template <typename EnumType>
    struct EntityBase {
        bool is_active_ = false;
        EnumType type_{};
        float x_ = 0.0f;
        float y_ = 0.0f;
        float vx_ = 0.0f;
        float vy_ = 0.0f;
        int elapsed_time_ = 0;
        int animation_time_ = 0;
    };

    // エンティティマネージャーのテンプレートクラス
    template <typename EntityType, typename EnumType, size_t PoolSize>
    class EntityManager {
      public:
        static constexpr size_t kPoolSize = PoolSize;
        static constexpr size_t kTypeSize = static_cast<size_t>(EnumType::ENUM_END);

        EntityManager() = default;
        virtual ~EntityManager() = default;

        void Init() {
            for (auto &entity : entities) {
                entity.is_active_ = false;
            }
        }

        void Update() {
            const auto &properties = GetSpriteProperties();

            for (auto &entity : entities_) {
                if (!entity.is_active_) continue;

                // 派生クラスの固有処理を呼び出す
                UniqueUpdate(entity);

                // 共通変数の更新
                entity.x_ += entity.vx_;
                entity.y_ += entity.vy_;
                entity.elapsed_time_++;

                // アニメーション更新
                const auto &prop = properties[static_cast<size_t>(entity.type_)];
                if (prop.animation_num_ >= 2) {
                    entity.animation_time_++;
                    if (entity.animation_time_ / prop.animation_interval_ >= prop.animation_num_) {
                        switch (prop.animation_mode_) { // アニメーションモードに従って終端での処理を行う
                        case AnimationMode::Loop:
                            entity.animation_time_ = 0;
                            break;
                        case AnimationMode::Destroy:
                            entity.is_active_ = false;
                            break;
                        case AnimationMode::Hold:
                            entity.animation_time_ = prop.animation_interval_ * (prop.animation_num_ - 1);
                            break;
                        }
                    }
                } else {
                    // フレーム数が1枚のみの場合はアニメーションを考慮しない
                    entity.animation_time_ = 0;
                }
            }
        }

        void Draw() {
            const auto &properties = GetSpriteProperties();

            for (auto &entity : entities) {
                if (!entity.is_active_) continue;

                const auto &prop = properties[static_cast<size_t>(entity.type_)];
                int image_handle = asset.GetImageAsset(prop.image_id_);
                int image_height = 0;
                GetGraphSize(image_handle, nullptr, &image_height);

                int src_x = (entity.animation_time_ / prop.animation_interval_) * prop.frame_width_; // 現在フレームから画像アセットの描画範囲を算出する
                DrawRectRotaGraphF(entity.x_, entity.y_, src_x, 0, prop.frame_width_, image_height, 1.0, 0.0, image_handle, true);
            }
        }

        EntityType *Spawn(EnumType type, float x, float y) {
            for (auto &entity : entities_) {
                if (!entity.is_active_) { // 配列の空きを探して新規エンティティを追加する
                    entity = EntityType{};
                    entity.is_active_ = true;
                    entity.type_ = type;
                    entity.x_ = x;
                    entity.y_ = y;
                    UniqueSpawn(entity);
                    return &entity;
                }
            }
            return nullptr;
        }

        std::array<EntityType, PoolSize> &GetEntities() {
            return entities_;
        }

        size_t GetActiveCount() const {
            size_t count = 0;
            for (auto &entity : entities_) {
                if (entity.is_active_) {
                    count++;
                }
            }
            return count;
        }

      protected:
        virtual const std::array<SpriteProperty, kTypeSize> &GetSpriteProperties() const = 0;
        virtual void UniqueSpawn(EntityType &entity) {};
        virtual void UniqueUpdate(EntityType &entity) {};

      private:
        std::array<EntityType, PoolSize> entities_{};
    };
}

#endif
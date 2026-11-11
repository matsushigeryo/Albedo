#include "effect.h"
#include <iostream>
#include <DxLib.h>
#include "global.h"

namespace game {
    void EffectManager::Init() {
        for (auto &effect : effects_) {
            effect.is_active_ = false;
        }
    }

    void EffectManager::Update() {
        for (auto &effect : effects_) {
            if (effect.is_active_) {

                // エフェクト種別ごとに固有の更新をする
                UniqueUpdate(effect);

                // 座標更新
                effect.x_ += effect.vx_;
                effect.y_ += effect.vy_;

                // 時間経過
                effect.elapsed_time_++;

                // アニメーションのループ処理／終了処理
                const EffectSpriteProperty &property = kEffectSpriteProperties[static_cast<size_t>(effect.type_)];
                if (effect.elapsed_time_ / property.frame_animation_interval_ >= property.frame_count_) {
                    if (property.is_loop_) {
                        effect.elapsed_time_ = 0; // アニメーションループ
                    } else {
                        effect.is_active_ = false; // アニメーション終了
                    }
                }
            }
        }
    }

    void EffectManager::Draw() {
        int image_handle = -1;
        int image_height = 0;
        int src_x = 0;

        for (const auto &effect : effects_) {
            if (effect.is_active_) {
                const EffectSpriteProperty &property = kEffectSpriteProperties[static_cast<size_t>(effect.type_)];
                image_handle = asset.GetImageAsset(property.image_id_);                                                                // スプライトの画像ハンドル
                GetGraphSize(image_handle, nullptr, &image_height);                                                                    // 画像の高さを取得
                src_x = (effect.elapsed_time_ / property.frame_animation_interval_) * property.frame_width_;                           // 現在のアニメーションフレームより、元画像から切り出すオフセットを決定
                DrawRectRotaGraphF(effect.x_, effect.y_, src_x, 0, property.frame_width_, image_height, 1.0, 0.0, image_handle, true); // スプライト描画
            }
        }
    }

    bool EffectManager::Spawn(EffectType type, float x, float y) {
        for (auto &effect : effects_) {
            if (!effect.is_active_) { // 配列から空きを見つけてエフェクトを追加
                effect.is_active_ = true;
                effect.type_ = type;
                effect.x_ = x;
                effect.y_ = y;
                effect.vx_ = 0.0f;
                effect.vy_ = 0.0f;
                effect.elapsed_time_ = 0;
                UniqueSpawn(effect);
                return true;
            }
        }

        // エフェクトが追加できなかった場合
        // std::cerr << "エフェクトの追加に失敗しました。" << std::endl;
        return false;
    }

    // エフェクト種別ごとの固有の初期化をする
    void EffectManager::UniqueSpawn(Effect &effect) {

        switch (effect.type_) {
        case EffectType::EnemyDead: {
            const float angle = xoshiro.GetRandomNormalized() * kPi * 2.0f;
            const float speed = LerpF(2.0f, 3.5f, xoshiro.GetRandomNormalized());
            effect.vx_ = std::cosf(angle) * speed;
            effect.vy_ = std::sinf(angle) * speed;
            break;
        }

        default:
            break;
        }
    }

    // エフェクト種別ごとの固有の更新をする
    void EffectManager::UniqueUpdate(Effect &effect) {
        switch (effect.type_) {
        default:
            break;
        }
    }
}
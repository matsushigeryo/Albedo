#include "item.h"
#include <DxLib.h>
#include "global.h"
#include "player.h"
#include "text_entity.h"
#include "xoshiro.h"

namespace game {

    // プレイヤーに触れた時
    void ItemManager::OnHitPlayer(Item &item) {
        if (item.on_hit_player_flag_) return;
        item.on_hit_player_flag_ = true;
        item.on_hit_player_timer_ = kOnHitPlayerAnimDuration;
        item.on_hit_player_position_y_ = item.y_;
        // スコアの値をテキストエンティティとして追加する（現在無効化中）
        // TextEntity *text_entity = g_game.text_entity_manager->Spawn(TextEntityType::POPUP_SCORE, item.x_, item.y_);
        // if (text_entity) {
        //     text_entity->score_ = static_cast<unsigned int>(g_game.xoshiro->GetRandomNormalized() * 10000.0f);
        // }
    }

    // 固有のスポーン処理
    void ItemManager::UniqueSpawn(Item &item) {
        switch (item.type_) {
        case ItemType::Coin:
            item.vy_ = kStartVelocity;
            break;
        default:
            break;
        }
    }

    // 固有の更新処理
    void ItemManager::UniqueUpdate(Item &item) {
        switch (item.type_) {
        case ItemType::Coin:
            if (!item.on_hit_player_flag_) {
                // 重力を与える
                UpdateGravity(item);
            } else {
                // プレイヤー取得時の動作
                item.vx_ = 0.0f;
                item.vy_ = 0.0f;
                item.y_ = item.on_hit_player_position_y_ + static_cast<float>(kPlayerPickUpAnimationDistanceY) * EaseOutQuad(static_cast<float>(kOnHitPlayerAnimDuration - item.on_hit_player_timer_) / kOnHitPlayerAnimDuration);
                // タイマー更新
                item.on_hit_player_timer_--;
                if (item.on_hit_player_timer_ < 0) {
                    item.is_active_ = false; // 一定時間で削除
                }
            }
            break;
        default:
            break;
        }
    }

    // 固有の描画処理
    void ItemManager::UniqueDraw(Item &item, const SpriteProperty &prop, int image_handle, int src_x, int image_height) {
        // 描画座標を算出
        const float draw_x = item.x_;
        const float draw_y = item.y_;
        // スプライト描画
        SetDrawBright(0, 0, 0);
        DrawRectRotaGraphF(draw_x, draw_y + 1, src_x, 0, prop.image_width_, image_height, 1.0, 0.0, image_handle, true);
        SetDrawBright(255, 255, 255);
        DrawRectRotaGraphF(draw_x, draw_y, src_x, 0, prop.image_width_, image_height, 1.0, 0.0, image_handle, true);
        // 取得時は上から白塗りのスプライトを描画して点滅演出を行う
        if (item.on_hit_player_flag_ && item.on_hit_player_timer_ / kPlayerPickUpAnimationFlashInterval % 2 == 0) {
            SetIgnoreDrawGraphColor(true);
            DrawRectRotaGraphF(draw_x, draw_y, src_x, 0, prop.image_width_, image_height, 1.0, 0.0, image_handle, true);
            SetIgnoreDrawGraphColor(false);
        }
    }

    // 重力を与える処理
    void ItemManager::UpdateGravity(Item &item) {
        const float player_x = g_game.player->GetX();
        const float player_y = g_game.player->GetY();

        if (item.player_sticked_) {
            // プレイヤーへの吸着
            float dx = player_x - item.x_;
            float dy = player_y - item.y_;
            float dist = std::sqrtf((dx * dx) + (dy * dy));
            item.vx_ = (dx / dist) * kPlayerStickEffectSpeed;
            item.vy_ = (dy / dist) * kPlayerStickEffectSpeed;

        } else {
            // 落下
            item.vx_ *= 0.1f;
            item.vy_ += kAddVelocity;
            if (item.vy_ > kMaxVelocity) item.vy_ = kMaxVelocity;
            if (item.y_ > kGraphicHeight + 32) item.is_active_ = false;

            // プレイヤーへの吸着判定を立てる
            if (GetDistance(player_x, player_y, item.x_, item.y_) <= kPlayerStickEffectRadius) {
                item.player_sticked_ = true;
            }
        }
    }
}
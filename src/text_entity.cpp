#include "text_entity.h"
#include <DxLib.h>
#include "asset.h"
#include "game.h"
#include "global.h"
#include "text_function.h"

namespace game {

    void TextEntityManager::Update() {
        for (auto &entity : entities_) {
            if (!entity.is_active_) continue;

            switch (entity.type_) {
            case TextEntityType::POPUP_SCORE:
                entity.y_ = entity.start_y_ + EaseOutQuad(entity.elapsed_time_ / static_cast<float>(kPopupScoreLifeTime)) * kPopupScoreAnimationDistanceY; // ポップアップのアニメーション
                if (entity.elapsed_time_ >= kPopupScoreLifeTime) entity.is_active_ = false;                                                                // 時間経過で削除
                break;
            }

            entity.elapsed_time_++;
        }
    }

    void TextEntityManager::Draw() const {
        char buf[256] = "";
        unsigned int color = GetColor(255, 255, 255);

        for (auto &entity : entities_) {
            if (!entity.is_active_) continue;

            switch (entity.type_) {
            case TextEntityType::POPUP_SCORE:
                snprintf(buf, sizeof(buf), "+%d", entity.score_);                                                    // 描画用文字列に整形する
                int fade = 255 - InvLerpF(kPopupScoreLifeTime / 2, kPopupScoreLifeTime, entity.elapsed_time_) * 255; // フェード効果の値を計算
                if (fade < 0) fade = 0;
                if (fade > 255) fade = 255;
                SetDrawBlendMode(DX_BLENDMODE_ALPHA, fade);                                                          // フェードアウト効果
                color = GetColor(fade, fade, fade);                                                                  // グレーアウト効果
                TextFunction::DrawString(std::string(buf), entity.x_, entity.y_, FontID::PublicPixel8px, color, 0.5f, 0.5f);
                SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
                break;
            }
        }
    }

    TextEntity *TextEntityManager::Spawn(TextEntityType type, float x, float y) {
        for (auto &entity : entities_) {
            if (!entity.is_active_) {
                entity = TextEntity{};
                entity.is_active_ = true;
                entity.type_ = type;
                entity.x_ = x;
                entity.y_ = y;
                entity.start_x_ = x;
                entity.start_y_ = y;
                return &entity;
            }
        }
        return nullptr;
    }
}
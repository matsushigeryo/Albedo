#ifndef TEXT_ENTITY_H_
#define TEXT_ENTITY_H_

#include <cinttypes>
#include <array>
#include <DxLib.h>

namespace game {

    // テキストエンティティの種別
    enum class TextEntityType : uint8_t {
        POPUP_SCORE
    };

    // テキストエンティティの構造体
    struct TextEntity {
        bool is_active_ = false;
        TextEntityType type_;
        int elapsed_time_ = 0;
        float x_ = 0.0f;
        float y_ = 0.0f;
        float start_x_ = 0.0f;
        float start_y_ = 0.0f;
        unsigned int score_ = 0;
    };

    // テキストエンティティの管理クラス
    class TextEntityManager {
        static constexpr size_t kPoolSize = 256;
        static constexpr int kPopupScoreLifeTime = 60;
        static constexpr int kPopupScoreAnimationDistanceY = -26;

      public:
        TextEntityManager() = default;
        ~TextEntityManager() = default;
        void Update();
        void Draw() const;
        TextEntity *Spawn(TextEntityType type, float x, float y);

      private:
        std::array<TextEntity, kPoolSize> entities_{};
    };
}

#endif
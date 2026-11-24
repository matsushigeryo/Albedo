#ifndef ENEMY_H_
#define ENEMY_H_

#include <array>
#include "asset.h"
#include "enemy_script.h"
#include "entity_manager.h"

namespace game {

    constexpr int kEnemyOnDamageFlashEffectLength = 4;

    // 敵のスプライト種別
    enum class EnemySprite {
        Kuro,
        ENUM_END
    };

    // 文字列から敵のスプライト種別を取得する関数
    static EnemySprite EnemySpriteFromString(const std::string &str) {
        static const std::unordered_map<std::string, EnemySprite> kEnemySpriteMap = {
            {"Kuro", EnemySprite::Kuro},
        };
        if (kEnemySpriteMap.find(str) != kEnemySpriteMap.end()) {
            return kEnemySpriteMap.at(str);
        } else {
            return EnemySprite::Kuro; // 見つからなかった場合はデフォルト値を返す
        }
    }

    // 敵の構造体
    struct Enemy : public EntityBase<EnemySprite> {
        int on_damage_timer_ = 0;
        int life_ = 0;
        std::unique_ptr<EnemyScriptRunner> script_runner_{nullptr};
    };

    // 敵管理のクラス
    class EnemyManager : public EntityManager<Enemy, EnemySprite, 64> {
      public:
        using EntityManager::Spawn;

        Enemy *Spawn(EnemySprite sprite, float x, float y, std::string script_name, int life);
        void Die(Enemy &enemy);
        void OnHitPlayerBullet(Enemy &enemy);

      protected:
        const std::array<SpriteProperty, kTypeSize> &GetSpriteProperties() const override {
            static const std::array<SpriteProperty, kTypeSize> kProperties = {{
                {ImageID::EnemyKuro, 32, 1, 1}, // Kuro
            }};
            return kProperties;
        }

        void UniqueUpdate(Enemy &enemy) override;
        void UniqueDraw(Enemy &enemy, const SpriteProperty &prop, int image_handle, int src_x, int image_height) override;
    };
}

#endif
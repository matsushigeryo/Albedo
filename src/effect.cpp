#include "effect.h"
#include <iostream>
#include <DxLib.h>
#include "asset.h"
#include "game.h"
#include "global.h"
#include "xoshiro.h"

namespace game {

    // 固有のスポーン処理
    void EffectManager::UniqueSpawn(Effect &effect) {

        switch (effect.type_) {
        case EffectType::EnemyDead: {
            const float angle = g_game.xoshiro->GetRandomNormalized() * kPi * 2.0f;
            const float speed = LerpF(2.0f, 3.5f, g_game.xoshiro->GetRandomNormalized());
            effect.vx_ = std::cosf(angle) * speed;
            effect.vy_ = std::sinf(angle) * speed;
            break;
        }

        default:
            break;
        }
    }
}
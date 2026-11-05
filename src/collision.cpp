#include "collision.h"
#include "player_bullet_manager.h"
#include "enemy_manager.h"

namespace game {
    
    // 正円同士の当たり判定
    bool CheckCollisionCircleToCircle(float ax, float ay, float ar, float bx, float by, float br) {
        const float dx = bx - ax;
        const float dy = by - ay;
        const float distance = std::sqrtf((dx * dx) + (dy * dy));
        return distance < (ar + br);
    }

    // プレイヤー弾と敵の当たり判定
    void UpdateCollisionPlayerBulletToEnemy(PlayerBulletManager *player_bullet_manager, EnemyManager *enemy_manager) {
        if (!player_bullet_manager || !enemy_manager) return; // NULLチェック

        for (auto &bullet : player_bullet_manager->GetBullets()) {
            if (!bullet.is_active_) continue;
            for (auto &enemy : enemy_manager->GetEnemies()) {
                if (!enemy.is_active_) continue;
                if (CheckCollisionCircleToCircle(bullet.x_, bullet.y_, kPlayerBulletCollisionRadius, enemy.x_, enemy.y_, kEnemyCollisionRadius)) {
                    bullet.is_active_ = false;
                    break;
                }
            }
        }
    }
}
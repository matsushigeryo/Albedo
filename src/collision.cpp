#include "collision.h"
#include "player.h"
#include "player_bullet.h"
#include "enemy.h"
#include "enemy_bullet.h"
#include "item.h"

namespace game {

    // 正円同士の当たり判定
    bool CheckCollisionCircleToCircle(float ax, float ay, float ar, float bx, float by, float br) {
        const float dx = bx - ax;
        const float dy = by - ay;
        const float distance = std::sqrtf((dx * dx) + (dy * dy));
        return distance < (ar + br);
    }

    // プレイヤーと敵の当たり判定
    void UpdateCollisionPlayerToEnemy(Player *player, EnemyManager *enemy_manager) {
        if (!player || !enemy_manager) return; // NULLチェック

        for (auto &enemy : enemy_manager->GetEntities()) {
            if (!enemy.is_active_) continue;
            if (CheckCollisionCircleToCircle(player->GetX(), player->GetY(), kPlayerCollisionRadius, enemy.x_, enemy.y_, kEnemyCollisionRadius)) {
                player->OnDamage();
                break;
            }
        }
    }

    // プレイヤーと敵弾の当たり判定
    void UpdateCollisionPlayerToEnemyBullet(Player *player, EnemyBulletManager *enemy_bullet_manager) {
        if (!player || !enemy_bullet_manager) return; // NULLチェック

        for (auto &bullet : enemy_bullet_manager->GetEntities()) {
            if (!bullet.is_active_) continue;
            if (CheckCollisionCircleToCircle(player->GetX(), player->GetY(), kPlayerCollisionRadius, bullet.x_, bullet.y_, kEnemyBulletCollisionRadius)) {
                player->OnDamage();
                enemy_bullet_manager->OnHitPlayer(bullet);
                break;
            }
        }
    }

    // プレイヤー弾と敵の当たり判定
    void UpdateCollisionPlayerBulletToEnemy(PlayerBulletManager *player_bullet_manager, EnemyManager *enemy_manager) {
        if (!player_bullet_manager || !enemy_manager) return; // NULLチェック

        for (auto &bullet : player_bullet_manager->GetEntities()) {
            if (!bullet.is_active_) continue;
            for (auto &enemy : enemy_manager->GetEntities()) {
                if (!enemy.is_active_) continue;
                if (CheckCollisionCircleToCircle(bullet.x_, bullet.y_, kPlayerBulletCollisionRadius, enemy.x_, enemy.y_, kEnemyCollisionRadius)) {
                    player_bullet_manager->OnHitEnemy(bullet);
                    enemy_manager->OnHitPlayerBullet(enemy);
                    break;
                }
            }
        }
    }

    // プレイヤーとアイテムの当たり判定
    void UpdateCollisionPlayerToItem(Player* player, ItemManager* item_manager) {
        if (!player || !item_manager) return; // NULLチェック

        for (auto& item : item_manager->GetEntities()) {
            if (!item.is_active_) continue;
            if (CheckCollisionCircleToCircle(player->GetX(), player->GetY(), kPlayerCollisionRadius, item.x_, item.y_, kItemCollisionRadius)) {
                item.is_active_ = false;
                break;
            }
        }
    }
}
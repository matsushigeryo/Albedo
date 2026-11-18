#ifndef COLLISION_H_
#define COLLISION_H_

class Player;
class PlayerBulletManager;
class EnemyManager;
class EnemyBulletManager;
class ItemManager;

namespace game {
    constexpr float kPlayerCollisionRadius = 1;
    constexpr float kPlayerBulletCollisionRadius = 4;
    constexpr float kEnemyCollisionRadius = 5;
    constexpr float kEnemyBulletCollisionRadius = 1;
    constexpr float kItemCollisionRadius = 8;

    bool CheckCollisionCircleToCircle(float ax, float ay, float ar, float bx, float by, float br);
    void UpdateCollisionPlayerToEnemy(Player *player, EnemyManager *enemy_manager);
    void UpdateCollisionPlayerToEnemyBullet(Player *player, EnemyBulletManager *enemy_bullet_manager);
    void UpdateCollisionPlayerBulletToEnemy(PlayerBulletManager *player_bullet_manager, EnemyManager *enemy_manager);
    void UpdateCollisionPlayerToItem(Player *player, ItemManager *item_manager);
}

#endif
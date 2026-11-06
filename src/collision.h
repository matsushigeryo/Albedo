#ifndef COLLISION_H_
#define COLLISION_H_

class Player;
class PlayerBulletManager;
class EnemyManager;
class EnemyBulletManager;

namespace game {
    constexpr float kPlayerCollisionRadius = 1;
    constexpr float kPlayerBulletCollisionRadius = 4;
    constexpr float kEnemyCollisionRadius = 5;
    constexpr float kEnemyBulletCollisionRadius = 2;

    bool CheckCollisionCircleToCircle(float ax, float ay, float ar, float bx, float by, float br);
    void UpdateCollisionPlayerToEnemy(Player *player, EnemyManager *enemy_manager);
    void UpdateCollisionPlayerToEnemyBullet(Player *player, EnemyBulletManager *enemy_bullet_manager);
    void UpdateCollisionPlayerBulletToEnemy(PlayerBulletManager *player_bullet_manager, EnemyManager *enemy_manager);
}

#endif
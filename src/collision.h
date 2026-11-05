#ifndef COLLISION_H_
#define COLLISION_H_

class PlayerBulletManager;
class EnemyManager;

namespace game {
    constexpr float kEnemyCollisionRadius = 7;
    constexpr float kPlayerBulletCollisionRadius = 4;

    bool CheckCollisionCircleToCircle(float ax, float ay, float ar, float bx, float by, float br);
    void UpdateCollisionPlayerBulletToEnemy(PlayerBulletManager *player_bullet_manager, EnemyManager *enemy_manager);
}

#endif
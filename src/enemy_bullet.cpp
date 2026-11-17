#include "enemy_bullet.h"
#include <iostream>
#include <DxLib.h>
#include "asset.h"
#include "game.h"
#include "global.h"

namespace game {

    // “G’e‚ÌƒXƒ|[ƒ“
    EnemyBullet *EnemyBulletManager::Spawn(float x, float y, float angle, float speed, EnemyBulletSprite sprite) {
        auto *bullet = Spawn(sprite, x, y);

        if (bullet) {
            // ŒÅ—L‚Ìƒƒ“ƒo•Ï”‚ðÝ’è
            bullet->vx_ = cosf(angle) * speed;
            bullet->vy_ = sinf(angle) * speed;
            bullet->angle_ = angle;
            bullet->speed_ = speed;
        } else {
            // “G’e‚ª’Ç‰Á‚Å‚«‚È‚©‚Á‚½ê‡
            g_game.log->Print("EnemyBulletManager: “G’e‚ÌƒXƒ|[ƒ“‚ÉŽ¸”s‚µ‚Ü‚µ‚½");
        }

        return bullet;
    }

    // ƒvƒŒƒCƒ„[‚É“–‚½‚Á‚½‚Æ‚«
    void EnemyBulletManager::OnHitPlayer(EnemyBullet &bullet) {
        bullet.is_active_ = false; // ’e‚ðÁ‚·
    }

    // ŒÅ—L‚Ì•`‰æˆ—
    void EnemyBulletManager::UniqueDraw(EnemyBullet &bullet, const SpriteProperty &prop, int image_handle, int src_x, int image_height) {

        // ƒXƒ|[ƒ“Žž‚ÌƒAƒjƒ[ƒVƒ‡ƒ“
        float scale = 1.0f;
        float alpha = 1.0f;
        if (bullet.elapsed_time_ >= 0 && bullet.elapsed_time_ <= kEnemyBulletEnterEffectLength) {
            scale = 1.0f + InvLerpF(kEnemyBulletEnterEffectLength, 0, bullet.elapsed_time_);
            alpha = InvLerpF(0, kEnemyBulletEnterEffectLength, bullet.elapsed_time_);
        }

        // ƒXƒvƒ‰ƒCƒg•`‰æ
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha * 255);
        // SetDrawMode(DX_DRAWMODE_BILINEAR);
        DrawRectRotaGraphF(bullet.x_, bullet.y_, src_x, 0, prop.image_width_, image_height, scale, GetDrawAngle(bullet), image_handle, true);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
        // SetDrawMode(DX_DRAWMODE_NEAREST);
    }

    // •`‰æŠp“x‚ð’e‚²‚Æ‚ÉŒˆ’è‚·‚éŠÖ”
    float EnemyBulletManager::GetDrawAngle(const EnemyBullet &bullet) const {
        switch (bullet.type_) {
        case EnemyBulletSprite::SpinningRedRice:
            return bullet.elapsed_time_ * kEnemyBulletSpinningAnimationSpeed;
        default:
            return bullet.angle_ + (kPi / 2.0f);
        }
    }
}
#include "game.h"
#include <iostream>
#include <DxLib.h>
#include "enemy_script.h"
#include "global.h"
#include "collision.h"

namespace game {

    // 実体を定義
    int game_time = 0;
    Asset asset{"asset"};
    Input input{};
    Log log{"game.log"};
    Camera camera{};
    std::unique_ptr<TileMap> tilemap_ptr = nullptr;
    std::unique_ptr<Player> player_ptr = nullptr;
    std::unique_ptr<PlayerBulletManager> player_bullet_manager_ptr = nullptr;
    std::unique_ptr<EnemyManager> enemy_manager_ptr = nullptr;
    std::unique_ptr<EnemyBulletManager> enemy_bullet_manager_ptr = nullptr;
    std::unique_ptr<EnemyScriptManager> enemy_script_manager_ptr = nullptr;
    std::unique_ptr<EffectManager> effect_manager_ptr = nullptr;

    void Init() {
        // アセット群の読み込み
        asset.Clear();
        asset.LoadImageAsset(ImageID::TileSet, "image/tileset.png");
        asset.LoadImageAsset(ImageID::Player, "image/player.png");
        asset.LoadImageAsset(ImageID::PlayerOption, "image/player_option.png");
        asset.LoadImageAsset(ImageID::PlayerBulletMainShot, "image/player_bullet_main_shot.png");
        asset.LoadImageAsset(ImageID::PlayerBulletSubShot, "image/player_bullet_sub_shot.png");
        asset.LoadImageAsset(ImageID::EnemyKuro, "image/enemy_kuro.png");
        asset.LoadImageAsset(ImageID::EnemyBullet, "image/enemy_bullet.png");
        asset.LoadImageAsset(ImageID::EffectWhiteSparkle1, "image/effect_white_sparkle_1.png");
        asset.LoadImageAsset(ImageID::EffectWhiteSparkle2, "image/effect_white_sparkle_2.png");

        // 初期化
        tilemap_ptr = std::make_unique<TileMap>("asset/data/test_tilemap.tmj");
        player_ptr = std::make_unique<Player>();
        player_bullet_manager_ptr = std::make_unique<PlayerBulletManager>();
        enemy_manager_ptr = std::make_unique<EnemyManager>();
        enemy_bullet_manager_ptr = std::make_unique<EnemyBulletManager>();
        enemy_script_manager_ptr = std::make_unique<EnemyScriptManager>();
        effect_manager_ptr = std::make_unique<EffectManager>();

        camera.Init();
        player_ptr->Init();
        player_bullet_manager_ptr->Init();
        enemy_manager_ptr->Init();
        enemy_bullet_manager_ptr->Init();
        effect_manager_ptr->Init();
    }

    void Update() {
        if (input.OnPressedKeyboard(KEY_INPUT_F5)) {
            Init();
            return;
        }

        // ゲーム内要素の更新
        camera.Update();
        player_ptr->Update();
        player_bullet_manager_ptr->Update();
        enemy_manager_ptr->Update();
        enemy_bullet_manager_ptr->Update();
        effect_manager_ptr->Update();
        
        // 当たり判定の処理
        UpdateCollisionPlayerToEnemy(player_ptr.get(), enemy_manager_ptr.get());
        UpdateCollisionPlayerToEnemyBullet(player_ptr.get(), enemy_bullet_manager_ptr.get());
        UpdateCollisionPlayerBulletToEnemy(player_bullet_manager_ptr.get(), enemy_manager_ptr.get());

        if (game_time >= 40 && (game_time - 40) % 20 == 0) {
            enemy_manager_ptr->Spawn(EnemySprite::Kuro, 32 + rand() % (kGraphicWidth - 64), -32);
        }

        static EnemyScriptRunner test_runner(enemy_script_manager_ptr->Get("asset/data/test_script.xml"));
        test_runner.Update();

        game_time++;
    }

    void Draw() {
        tilemap_ptr->Draw();
        player_bullet_manager_ptr->Draw();
        player_ptr->Draw();
        enemy_manager_ptr->Draw();
        enemy_bullet_manager_ptr->Draw();
        effect_manager_ptr->Draw();
    }

    int GetGameTime() {
        return game_time;
    }
}
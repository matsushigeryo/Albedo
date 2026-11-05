#include "game.h"
#include <iostream>
#include <DxLib.h>
#include "global.h"
#include "collision.h"

namespace game {

    // 実体を定義
    int time = 0;
    Asset asset{"asset"};
    Input input{};
    Camera camera{};
    std::unique_ptr<Player> player_ptr = nullptr;
    std::unique_ptr<PlayerBulletManager> player_bullet_manager_ptr = nullptr;
    std::unique_ptr<EnemyManager> enemy_manager_ptr = nullptr;
    std::unique_ptr<TileMap> tilemap_ptr = nullptr;

    void Init() {
        // アセット群の読み込み
        asset.LoadImageAsset(ImageID::TileSet, "tileset.png");
        asset.LoadImageAsset(ImageID::Player, "player.png");
        asset.LoadImageAsset(ImageID::PlayerOption, "player_option.png");
        asset.LoadImageAsset(ImageID::PlayerBulletMainShot, "player_bullet_main_shot.png");
        asset.LoadImageAsset(ImageID::PlayerBulletSubShot, "player_bullet_sub_shot.png");
        asset.LoadImageAsset(ImageID::EnemyKuro, "enemy_kuro.png");

        // 初期化
        tilemap_ptr = std::make_unique<TileMap>("asset/tilemap.tmj");
        player_ptr = std::make_unique<Player>();
        player_bullet_manager_ptr = std::make_unique<PlayerBulletManager>();
        enemy_manager_ptr = std::make_unique<EnemyManager>();

        camera.Init();
        player_ptr->Init();
        player_bullet_manager_ptr->Init();
        enemy_manager_ptr->Init();

        std::cout << "―― ゲームシーンを初期化しました ――" << std::endl;

        enemy_manager_ptr->Spawn(EnemySprite::Kuro, 30, 30);
        enemy_manager_ptr->Spawn(EnemySprite::Kuro, 60, 30);
        enemy_manager_ptr->Spawn(EnemySprite::Kuro, 90, 30);
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
        
        // 当たり判定の処理
        UpdateCollisionPlayerBulletToEnemy(player_bullet_manager_ptr.get(), enemy_manager_ptr.get());

        time++;
    }

    void Draw() {
        tilemap_ptr->Draw();
        player_bullet_manager_ptr->Draw();
        player_ptr->Draw();
        enemy_manager_ptr->Draw();
    }
}
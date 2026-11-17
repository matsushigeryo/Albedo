#include "game.h"
#include <DxLib.h>
#include <iostream>
#include "asset.h"
#include "camera.h"
#include "collision.h"
#include "effect.h"
#include "enemy.h"
#include "enemy_bullet.h"
#include "enemy_script.h"
#include "global.h"
#include "input.h"
#include "item.h"
#include "log.h"
#include "player.h"
#include "player_bomb.h"
#include "player_bullet.h"
#include "tilemap.h"
#include "xoshiro.h"

namespace game {

    Game::Game() = default;
    Game::~Game() = default;
    Game g_game{};

    std::unique_ptr<EnemyScriptRunner> test_runner_ptr = nullptr;
    float test_runner_origin_x = kGraphicWidth / 2.0f;
    float test_runner_origin_y = 32.0f;

    void Init() {
        // ポインタに実体を入れる
        g_game.asset = std::make_unique<Asset>("asset");
        g_game.xoshiro = std::make_unique<Xoshiro128PlusPlus>(0);
        g_game.input = std::make_unique<Input>();
        g_game.log = std::make_unique<Log>("game.log");
        g_game.camera = std::make_unique<Camera>();
        g_game.tilemap = std::make_unique<TileMap>("asset/data/test_tilemap.tmj");
        g_game.player = std::make_unique<Player>();
        g_game.player_bomb = std::make_unique<PlayerBomb>(5);
        g_game.player_bullet_manager = std::make_unique<PlayerBulletManager>();
        g_game.enemy_manager = std::make_unique<EnemyManager>();
        g_game.enemy_bullet_manager = std::make_unique<EnemyBulletManager>();
        g_game.enemy_script_manager = std::make_unique<EnemyScriptManager>();
        g_game.item_manager = std::make_unique<ItemManager>();
        g_game.effect_manager = std::make_unique<EffectManager>();

        // アセット群の読み込み
        g_game.asset->Clear();
        g_game.asset->LoadImageAsset(ImageID::TileSet, "image/tileset.png");
        g_game.asset->LoadImageAsset(ImageID::Player, "image/player.png");
        g_game.asset->LoadImageAsset(ImageID::PlayerOption, "image/player_option.png");
        g_game.asset->LoadImageAsset(ImageID::PlayerBulletMainShot, "image/player_bullet_main_shot.png");
        g_game.asset->LoadImageAsset(ImageID::PlayerBulletSubShot, "image/player_bullet_sub_shot.png");
        g_game.asset->LoadImageAsset(ImageID::EnemyKuro, "image/enemy_kuro.png");
        g_game.asset->LoadImageAsset(ImageID::EnemyBullet, "image/enemy_bullet.png");
        g_game.asset->LoadImageAsset(ImageID::ItemCoin, "image/item_coin.png");
        g_game.asset->LoadImageAsset(ImageID::EffectWhiteSparkle1, "image/effect_white_sparkle_1.png");
        g_game.asset->LoadImageAsset(ImageID::EffectWhiteSparkle2, "image/effect_white_sparkle_2.png");

        // 初期化
        g_game.input->Init();
        g_game.player->Init();
        g_game.player_bullet_manager->Init();
        g_game.enemy_manager->Init();
        g_game.enemy_bullet_manager->Init();
        g_game.item_manager->Init();
        g_game.effect_manager->Init();

        // test_runner_ptr = std::make_unique<EnemyScriptRunner>(g_game.enemy_script_manager->Get("asset/data/test_script.xml"));
        // test_runner_origin_x = kGraphicWidth / 2.0f;
        // test_runner_origin_y = 32.0f;

        g_game.log->Print("Game: ゲームシーンを初期化しました");
    }

    void Update() {
        if (g_game.input->OnPressedKeyboard(KEY_INPUT_F5)) {
            Init();
            return;
        }

        // ゲーム内要素の更新
        g_game.input->Update();
        g_game.camera->Update();
        g_game.tilemap->UpdateEnemySpawning();
        g_game.player->Update();
        g_game.player_bomb->Update();
        g_game.player_bullet_manager->Update();
        g_game.enemy_manager->Update();
        g_game.enemy_bullet_manager->Update();
        g_game.item_manager->Update();
        g_game.effect_manager->Update();

        // 当たり判定の処理
        UpdateCollisionPlayerToEnemy(g_game.player.get(), g_game.enemy_manager.get());
        UpdateCollisionPlayerToEnemyBullet(g_game.player.get(), g_game.enemy_bullet_manager.get());
        UpdateCollisionPlayerBulletToEnemy(g_game.player_bullet_manager.get(), g_game.enemy_manager.get());

        // 敵のスポーン処理(仮)
        // if (game_time >= 40 && (game_time - 40) % 40 == 0) {
        //     enemy_manager_ptr->Spawn(EnemySprite::Kuro, 32 + rand() % (kGraphicWidth - 64), -32, "test_script");
        // }

        if (test_runner_ptr) {
            static bool owner_is_active = true;
            EnemyScriptRunnerContext context{test_runner_origin_x, test_runner_origin_y, owner_is_active};
            test_runner_ptr->Update(context);
        }

        g_game.time++;
    }

    void Draw() {
        // ゲーム内要素の描画
        g_game.tilemap->Draw();
        g_game.player_bullet_manager->Draw();
        g_game.item_manager->Draw();
        g_game.player->Draw();
        g_game.enemy_manager->Draw();
        g_game.enemy_bullet_manager->Draw();
        g_game.effect_manager->Draw();
        g_game.player_bomb->Draw();
    }
}
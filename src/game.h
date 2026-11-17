#ifndef GAME_H_
#define GAME_H_

#include <memory>

namespace game {
    constexpr float kWorldScale = 16;

    class Xoshiro128PlusPlus;
    class Asset;
    class Input;
    class Log;
    class Camera;
    class TileMap;
    class Player;
    class PlayerBomb;
    class PlayerBulletManager;
    class EnemyManager;
    class EnemyBulletManager;
    class EnemyScriptManager;
    class ItemManager;
    class EffectManager;

    struct Game {
        Game();
        ~Game();

        int time = 0;
        std::unique_ptr<Xoshiro128PlusPlus> xoshiro;
        std::unique_ptr<Asset> asset;
        std::unique_ptr<Input> input;
        std::unique_ptr<Log> log;
        std::unique_ptr<Camera> camera;
        std::unique_ptr<TileMap> tilemap;
        std::unique_ptr<Player> player;
        std::unique_ptr<PlayerBomb> player_bomb;
        std::unique_ptr<PlayerBulletManager> player_bullet_manager;
        std::unique_ptr<EnemyManager> enemy_manager;
        std::unique_ptr<EnemyBulletManager> enemy_bullet_manager;
        std::unique_ptr<EnemyScriptManager> enemy_script_manager;
        std::unique_ptr<ItemManager> item_manager;
        std::unique_ptr<EffectManager> effect_manager;
    };

    extern Game g_game;

    void Init();
    void Update();
    void Draw();
}

#endif
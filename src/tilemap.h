#ifndef TILEMAP_H_
#define TILEMAP_H_

#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <enemy.h>
#include <game.h>

namespace game {
    class TileMap {
      public:
        static constexpr int kTileSetXNum = 16;
        static constexpr int kTileSetYNum = 16;
        static constexpr int kTileSetXSize = 16;
        static constexpr int kTileSetYSize = 16;
        static constexpr int kTileDrawWidth = 16;
        static constexpr int kTileDrawHeight = 16;

        struct Layer {
            Layer(const std::vector<unsigned int> &data, const std::string &name, const int width, const int height)
                : data_{data}, name_{name}, width_{width}, height_{height} {
            }

            std::vector<unsigned int> data_;
            std::string name_;
            int width_;
            int height_;
        };

        struct EnemySpawnPoint {
            EnemySpawnPoint(float x, float y, const std::string &script, EnemySprite sprite, int life)
                : x_{x}, y_{y}, script_{script}, sprite_{sprite}, life_{life} {
            }

            bool is_spawned_ = false;
            float x_;
            float y_;
            std::string script_;
            EnemySprite sprite_;
            int life_;
        };

      public:
        TileMap(const std::string &json_path);
        ~TileMap() = default;
        void UpdateEnemySpawning();
        bool Draw() const;
        int GetMapWidth() const;
        int GetMapHeight() const;

      private:
        bool OpenMapFile(const std::string &json_path);
        void LoadObjects(const nlohmann::json &object_layer);

      private:
        const std::string json_path_;
        int map_width_ = 0;
        int map_height_ = 0;
        std::vector<Layer> layers_ = {};
        std::vector<EnemySpawnPoint> enemy_spawn_points_ = {};
    };
}

#endif
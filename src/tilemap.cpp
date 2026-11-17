#include "tilemap.h"
#include <fstream>
#include <iostream>
#include <DxLib.h>
#include <magic_enum/magic_enum.hpp>
#include <nlohmann/json.hpp>
#include "camera.h"
#include "enemy.h"
#include "game.h"
#include "global.h"
#include "log.h"

namespace game {
    TileMap::TileMap(const std::string &json_path)
        : json_path_{json_path} {
        OpenMapFile(json_path);
    }

    void TileMap::UpdateEnemySpawning() {
        for (auto &enemy : enemy_spawn_points_) {
            if (!enemy.is_spawned_) {
                if (enemy.y_ > g_game.camera->GetY() - 1.0f) {
                    enemy.is_spawned_ = true;
                    g_game.enemy_manager->Spawn(
                        enemy.sprite_,
                        enemy.x_ * kWorldScale,
                        (enemy.y_ - g_game.camera->GetY()) * kWorldScale,
                        enemy.script_);
                }
            }
        }
    }

    bool TileMap::Draw() const {
        int tileset_image_handle = g_game.asset->GetImageAsset(ImageID::TileSet);
        if (tileset_image_handle == -1) return false;

        // タイルを描画する
        unsigned int tile_id, tile_x, tile_y;
        float dest_x1, dest_y1, dest_x2, dest_y2, src_x, src_y;
        const int camera_offset_int_x = std::floor(g_game.camera->GetX());
        const int camera_offset_int_y = std::floor(g_game.camera->GetY());
        const float camera_offset_x = g_game.camera->GetX() - camera_offset_int_x;
        const float camera_offset_y = g_game.camera->GetY() - camera_offset_int_y;

        for (auto &layer : layers_) {
            for (int y = 0; y < kGraphicHeight / kTileDrawHeight + 2; ++y) {
                for (int x = 0; x < kGraphicWidth / kTileDrawWidth + 2; ++x) {
                    tile_x = x + camera_offset_int_x;
                    tile_y = y + camera_offset_int_y;
                    if (tile_x >= 0 && tile_x < layer.width_ && tile_y >= 0 && tile_y < layer.height_) {
                        tile_id = layer.data_[tile_y * layer.width_ + tile_x];
                        if (tile_id != 0 && tile_id < static_cast<unsigned int>(kTileSetXNum * kTileSetYNum)) {
                            dest_x1 = (static_cast<float>(x) - camera_offset_x) * kTileDrawWidth;
                            dest_y1 = (static_cast<float>(y) - camera_offset_y) * kTileDrawHeight;
                            dest_x2 = dest_x1 + kTileDrawWidth;
                            dest_y2 = dest_y1 + kTileDrawHeight;
                            src_x = (tile_id - 1) % kTileSetXNum * kTileSetXSize;
                            src_y = (tile_id - 1) / kTileSetXNum * kTileSetYSize;
                            DrawRectExtendGraphF(dest_x1, dest_y1, dest_x2, dest_y2, src_x, src_y, kTileSetXSize, kTileSetYSize, tileset_image_handle, true);
                        }
                    }
                }
            }
        }

        return true;
    }

    bool TileMap::OpenMapFile(const std::string &json_path) {

        // ファイルを開く
        std::ifstream ifs(json_path);
        if (!ifs.is_open()) {
            g_game.log->Print("TileMap: タイルマップデータを開けませんでした: %s", json_path.c_str());
            return false;
        }

        // JSONをパース
        nlohmann::json json;
        ifs >> json;

        try {
            // マッププロパティを取得
            map_width_ = json.at("width").get<int>();
            map_height_ = json.at("height").get<int>();
            nlohmann::json layers = json.at("layers").get<nlohmann::json>();

            // 全てのレイヤーを取得
            layers_.clear();
            enemy_spawn_points_.clear();

            for (auto &layer : layers) {
                const std::string type = layer.at("type").get<std::string>();

                // タイルマップの場合
                if (type == "tilelayer") {
                    const std::string name = layer.at("name").get<std::string>();
                    layers_.emplace_back(
                        layer.at("data").get<std::vector<unsigned int>>(),
                        name,
                        layer.at("width").get<int>(),
                        layer.at("height").get<int>());
                    g_game.log->Print("TileMap: 地形レイヤーを読み込みました: %s", name.c_str());
                }

                // オブジェクトグループの場合
                else if (type == "objectgroup") {
                    LoadObjects(layer);
                }

                else {
                    g_game.log->Print("TileMap: タイルマップデータの内部に未知のレイヤータイプが見つかったため無視されます: %s", type.c_str());
                }
            }

            // カメラ初期座標の上端より下に存在する敵を削除する
            for (auto &enemy : enemy_spawn_points_) {
                if (enemy.y_ > g_game.camera->GetY()) {
                    enemy.is_spawned_ = true;
                }
            }

        } catch (const nlohmann::json::out_of_range &) {
            g_game.log->Print("TileMap: タイルマップデータの解析に失敗しました: %s", json_path.c_str());
            return false;
        }

        g_game.log->Print("TileMap: タイルマップを読み込みました: %s", json_path.c_str());

        return true;
    }

    void TileMap::LoadObjects(const nlohmann::json &object_layer) {
        const std::string layer_name = object_layer.at("name").get<std::string>();

        // 雑魚敵グループの場合
        if (layer_name == "enemy") {
            for (auto &object : object_layer.at("objects").get<std::vector<nlohmann::json>>()) {
                const std::string object_name = object.at("name").get<std::string>();
                const float object_x = object.at("x").get<float>() / kWorldScale;
                const float object_y = object.at("y").get<float>() / kWorldScale;
                const float object_width = object.at("width").get<float>() / kWorldScale;
                const float object_height = object.at("height").get<float>() / kWorldScale;

                // カスタムプロパティを参照
                std::string object_script = "";
                EnemySprite object_sprite = EnemySprite::Kuro;

                const std::vector<nlohmann::json> properties = object.at("properties").get<std::vector<nlohmann::json>>();

                for (auto &property : properties) {
                    const std::string property_name = property.at("name").get<std::string>();
                    const std::string property_type = property.at("type").get<std::string>();

                    if (property_name == "script" && property_type == "string") {
                        object_script = property.at("value").get<std::string>();
                    }

                    else if (property_name == "sprite" && property_type == "string") {
                        object_sprite = EnemySpriteFromString(property.at("value").get<std::string>());
                    }

                    else {
                        g_game.log->Print("TileMap: 敵オブジェクトから不明なカスタムプロパティが見つかったため無視されます: %s", property_name.c_str());
                    }
                }

                // 出現情報をまとめた構造体を追加する
                const float object_spawn_x = object_x + object_width / 2.0f;
                const float object_spawn_y = object_y + object_height / 2.0f;
                enemy_spawn_points_.emplace_back(
                    object_spawn_x,
                    object_spawn_y,
                    object_script,
                    object_sprite);

                g_game.log->Print("TileMap: 敵の出現情報を登録しました: (x=%f,y=%f,script=\"%s\",sprite=\"%s\")", object_spawn_x, object_spawn_y, object_script.c_str(), magic_enum::enum_name(object_sprite).data());
            }
        }

        // その他のオブジェクトグループの場合
        else if (layer_name == "others") {
            for (auto &object : object_layer.at("objects").get<std::vector<nlohmann::json>>()) {
                const std::string object_name = object.at("name").get<std::string>();
                const float object_x = object.at("x").get<float>() / kWorldScale;
                const float object_y = object.at("y").get<float>() / kWorldScale;

                // カメラの位置指定オブジェクト
                if (object_name == "camera") {
                    g_game.camera->Init(object_x, object_y);
                    g_game.log->Print("TileMap: カメラの位置を(%f,%f)で初期化しました", object_x, object_y);
                }

                else {
                    g_game.log->Print("TileMap: タイルマップデータの内部に未知のオブジェクト種別が見つかったため無視されます: %s", object_name.c_str());
                }
            }
        }

        else {
            g_game.log->Print("TileMap: タイルマップデータの内部に未知のオブジェクトグループが見つかったため無視されます: %s", layer_name.c_str());
        }
    }

    int TileMap::GetMapWidth() const {
        return map_width_;
    }

    int TileMap::GetMapHeight() const {
        return map_height_;
    }
}

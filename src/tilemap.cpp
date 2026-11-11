#include "tilemap.h"
#include <fstream>
#include <iostream>
#include <DxLib.h>
#include "nlohmann/json.hpp"
#include "camera.h"
#include "global.h"

namespace game {
    TileMap::TileMap(const std::string &json_path)
        : json_path_{json_path} {
        OpenMapFile(json_path);
        tileset_image_handle_ = asset.GetImageAsset(ImageID::TileSet);
    }

    bool TileMap::Draw() const {
        if (tileset_image_handle_ == -1) return false;

        // タイルを描画する
        unsigned int tile_id, tile_x, tile_y;
        float dest_x1, dest_y1, dest_x2, dest_y2, src_x, src_y;
        const int camera_offset_int_x = std::floor(camera.GetX());
        const int camera_offset_int_y = std::floor(camera.GetY());
        const float camera_offset_x = camera.GetX() - camera_offset_int_x;
        const float camera_offset_y = camera.GetY() - camera_offset_int_y;

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
                            DrawRectExtendGraphF(dest_x1, dest_y1, dest_x2, dest_y2, src_x, src_y, kTileSetXSize, kTileSetYSize, tileset_image_handle_, true);
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
            for (auto &layer : layers) {
                layers_.emplace_back(
                    layer.at("data").get<std::vector<unsigned int>>(),
                    layer.at("name").get<std::string>(),
                    layer.at("width").get<int>(),
                    layer.at("height").get<int>());
            }

        } catch (const nlohmann::json::out_of_range &) {
            return false;
        }

        return true;
    }

    int TileMap::GetMapWidth() const {
        return map_width_;
    }

    int TileMap::GetMapHeight() const {
        return map_height_;
    }
}

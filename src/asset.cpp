#include "asset.h"
#include <DxLib.h>
#include <magic_enum/magic_enum.hpp>
#include "game.h"
#include "log.h"

namespace game {
    Asset::Asset(const std::string &root_folder_path)
        : root_folder_path_{root_folder_path} {
    }

    // アセットを解放する
    void Asset::Clear() {
        for (const auto &pair : image_map_) {
            DeleteGraph(pair.second);
        }
        image_map_.clear();

        for (const auto &pair : font_map_) {
            DeleteFontToHandle(pair.second);
        }
        font_map_.clear();
    }

    // 全てのアセットを一括で読み込む
    bool Asset::Load() {
        bool success = true;
        
        for (size_t i = 0; i < kImageAssetSize; i++) {
            const ImageID image_id = static_cast<ImageID>(i);
            const std::string file_path = GetImageAssetPath(image_id);
            if (!LoadImageAsset(image_id, file_path)) {
                success = false;
            }
        }

        for (size_t i = 0; i < kFontAssetSize; i++) {
            const FontID font_id = static_cast<FontID>(i);
            const std::string file_path = GetFontAssetPath(font_id);
            if (!LoadFontAsset(font_id, file_path)) {
                success = false;
            }
        }

        return success;
    }

    // 画像ファイルを読み込む
    bool Asset::LoadImageAsset(const ImageID image_id, const std::string &file_path) {
        const std::string full_path = root_folder_path_ + "/" + file_path;
        int handle = LoadGraph(full_path.c_str(), false);
        if (handle == -1) {
            g_game.log->Print("Asset: 画像ファイルの読み込みに失敗しました: %s", full_path.c_str());
            return false;
        } else {
            image_map_[image_id] = handle;
            g_game.log->Print("Asset: 画像ファイルを読み込みました: %s", full_path.c_str());
            return true;
        }
    }

    // 画像アセットを取得する
    int Asset::GetImageAsset(const ImageID image_id) const {
        try {
            return image_map_.at(image_id);
        } catch (const std::out_of_range &) {
            g_game.log->Print("Asset: 取得しようとした画像アセットは存在しませんでした: ImageID::%s", magic_enum::enum_name(image_id).data());
            return -1;
        }
    }

    // フォントファイルを読み込む
    bool Asset::LoadFontAsset(const FontID font_id, const std::string &file_path) {
        const std::string full_path = root_folder_path_ + "/" + file_path;
        int handle = LoadFontDataToHandle(full_path.c_str(), 0);
        if (handle == -1) {
            g_game.log->Print("Asset: フォントファイルの読み込みに失敗しました: %s", full_path.c_str());
            return false;
        } else {
            font_map_[font_id] = handle;
            g_game.log->Print("Asset: フォントファイルを読み込みました: %s", full_path.c_str());
            return true;
        }
    }

    // フォントアセットを取得する
    int Asset::GetFontAsset(const FontID font_id) const {
        try {
            return font_map_.at(font_id);
        } catch (const std::out_of_range &) {
            g_game.log->Print("Asset: 取得しようとしたフォントアセットは存在しませんでした: FontID::%s", magic_enum::enum_name(font_id).data());
            return -1;
        }
    }
}
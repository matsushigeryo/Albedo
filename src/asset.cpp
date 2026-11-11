#include "asset.h"
#include <DxLib.h>
#include <magic_enum/magic_enum.hpp>
#include "global.h"

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
    }

    // 画像ファイルを読み込む
    bool Asset::LoadImageAsset(const ImageID image_id, const std::string &file_path) {
        const std::string full_path = root_folder_path_ + "/" + file_path;
        int handle = LoadGraph(full_path.c_str(), false);
        if (handle == -1) {
            log.Print("Asset: 画像ファイルの読み込みに失敗しました: %s", full_path.c_str());
            return false;
        } else {
            image_map_[image_id] = handle;
            log.Print("Asset: 画像ファイルを読み込みました: %s", full_path.c_str());
            return true;
        }
    }

    // 画像アセットを取得する
    int Asset::GetImageAsset(const ImageID image_id) const {
        try {
            return image_map_.at(image_id);
        } catch (const std::out_of_range &) {
            log.Print("Asset: 取得しようとした画像アセットは存在しませんでした: ImageID::%s", magic_enum::enum_name(image_id).data());
            return -1;
        }
    }
}
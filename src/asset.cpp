#include "asset.h"
#include <iostream>
#include <DxLib.h>

namespace game {
    Asset::Asset(const std::string &root_folder_path)
        : root_folder_path_{root_folder_path} {
    }

    // 画像ファイルを読み込む
    bool Asset::LoadImageAsset(const ImageID image_id, const std::string &file_path) {
        const std::string full_path = root_folder_path_ + "/" + file_path;
        int handle = LoadGraph(full_path.c_str(), false);
        if (handle == -1) {
            std::cerr << "画像ファイルの読み込みに失敗しました: " << file_path << std::endl;
            return false;
        } else {
            image_map_[image_id] = handle;
            return true;
        }
    }

    // 画像アセットを取得する
    int Asset::GetImageAsset(const ImageID image_id) const {
        try {
            return image_map_.at(image_id);
        } catch (const std::out_of_range &e) {
            std::cerr << "取得しようとした画像アセットは存在しませんでした" << std::endl
                      << e.what() << std::endl;
        }
    }
}
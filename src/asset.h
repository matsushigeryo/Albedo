#ifndef ASSET_H_
#define ASSET_H_

#include <string>
#include <unordered_map>

namespace game {
    enum class ImageID {
        TileSet,
        Player,
        PlayerOption,
        PlayerBulletMainShot,
        PlayerBulletSubShot,
        EnemyKuro,
        EffectWhiteSparkle1,
        EffectWhiteSparkle2,
    };

    class Asset {
      public:
        Asset(const std::string &root_folder_path);
        ~Asset() = default;
        bool LoadImageAsset(const ImageID image_id, const std::string &file_path);
        int GetImageAsset(const ImageID image_id) const;

      private:
        std::string root_folder_path_;
        std::unordered_map<ImageID, int> image_map_;
    };
}

#endif
#ifndef ASSET_H_
#define ASSET_H_

#include <array>
#include <string>
#include <unordered_map>

namespace game {

    // 画像アセットの識別子
    enum class ImageID {
        TileSet,
        Player,
        PlayerOption,
        PlayerBulletMainShot,
        PlayerBulletSubShot,
        EnemyKuro,
        EnemyBullet_WhiteSmallCircle,
        EnemyBullet_SpinningRedRice,
        EnemyBullet_GreenNeedle,
        ItemCoin,
        EffectWhiteSparkle1,
        EffectWhiteSparkle2,
        ENUM_END
    };

    constexpr size_t kImageAssetSize = static_cast<size_t>(ImageID::ENUM_END);

    // 画像アセットのパスを取得する関数
    static std::string GetImageAssetPath(const ImageID image_id) {
        static std::array<std::string, kImageAssetSize> kImageAssetPaths = {
            "image/tileset.png",
            "image/player.png",
            "image/player_option.png",
            "image/player_bullet_main_shot.png",
            "image/player_bullet_sub_shot.png",
            "image/enemy_kuro.png",
            "image/enemy_bullet/white_small_circle.png",
            "image/enemy_bullet/spinning_red_rice.png",
            "image/enemy_bullet/green_needle.png",
            "image/item_coin.png",
            "image/effect_white_sparkle_1.png",
            "image/effect_white_sparkle_2.png",
        };
        return kImageAssetPaths[static_cast<size_t>(image_id)];
    }

    // アセット管理クラス
    class Asset {
      public:
        Asset(const std::string &root_folder_path);
        ~Asset() = default;
        void Clear();
        bool Load();
        bool LoadImageAsset(const ImageID image_id, const std::string &file_path);
        int GetImageAsset(const ImageID image_id) const;

      private:
        std::string root_folder_path_;
        std::unordered_map<ImageID, int> image_map_;
    };
}

#endif
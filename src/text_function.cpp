#include "text_function.h"
#include <DxLib.h>
#include <asset.h>
#include <game.h>

namespace game {

    // フォント、描画色、ピボットを指定して任意のテキストを描画する
    bool TextFunction::DrawString(const std::string& text, float x, float y, FontID font_id, unsigned int color, float pivot_x, float pivot_y) {
        if (text.empty()) return false;
        
        int font_handle = g_game.asset->GetFontAsset(font_id);
        if (font_handle == -1) return false;

        int size_x, size_y;
        GetDrawStringSizeToHandle(&size_x, &size_y, NULL, text.c_str(), text.size(), font_handle);

        DrawStringFToHandle(x - size_x * pivot_x, y - size_y * pivot_y, text.c_str(), color, font_handle);
        return true;
    }
}
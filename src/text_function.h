#ifndef TEXT_FUNCTION_H_
#define TEXT_FUNCTION_H_

#include <string>
#include <asset.h>

namespace game {
    
    // テキストの描画をサポートする静的関数群クラス
    class TextFunction {
      public:
        static bool DrawString(const std::string &text, float x, float y, FontID font_id, unsigned int color = 0xFFFFFFFF, float pivot_x = 0.0f, float pivot_y = 0.0f);
    };
}

#endif
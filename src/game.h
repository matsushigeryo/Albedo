#ifndef GAME_H_
#define GAME_H_

#include <memory>

namespace game {
    constexpr float kWorldScale = 16;

    void Init();
    void Update();
    void Draw();
    int GetGameTime();
}

#endif
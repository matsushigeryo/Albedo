#ifndef GAME_H_
#define GAME_H_

#include <memory>
#include "tilemap.h"

namespace game {
    void Init();
    void Update();
    void Draw();
    int GetGameTime();
}

#endif
#ifndef PLAYER_BOMB_H_
#define PLAYER_BOMB_H_

namespace game {
    class PlayerBomb {
        static constexpr int kDurationTime = 60 * 3;

      public:
        PlayerBomb(int init_stock);
        ~PlayerBomb() = default;
        void Update();
        void Draw();

      private:
        const int init_stock_;
        int stock_;
        int left_timer_ = 0;
    };
}

#endif
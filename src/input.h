#ifndef INPUT_H_
#define INPUT_H_

namespace game {
    class Input {
      public:
        Input() = default;
        ~Input() = default;
        void Init();
        void Update();
        bool OnHeldKeyboard(const unsigned char key_code) const;
        bool OnPressedKeyboard(const unsigned char key_code) const;
        bool OnReleasedKeyboard(const unsigned char key_code) const;

      private:
        char keyboard_current_state_[256]{};
        char keyboard_previous_state_[256]{};
    };
}

#endif
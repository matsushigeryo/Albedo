#ifndef CAMERA_H_
#define CAMERA_H_

namespace game {
    class Camera {
      public:
        Camera() = default;
        ~Camera() = default;
        void Init();
        void Update();
        float GetX() const;
        float GetY() const;

      private:
        float x_ = 0.0f;
        float y_ = 0.0f;
    };
}

#endif
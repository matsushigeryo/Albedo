#ifndef XORSHIRO_H_
#define XORSHIRO_H_

#include <cstdint>

class Xoshiro128PlusPlus {
  public:
    Xoshiro128PlusPlus(uint32_t seed) {
        // ó‘Ô‚Ì‰Šú‰»
        s[0] = seed;
        s[1] = seed ^ 0x9E3779B9;
        s[2] = seed ^ 0x3C6EF372;
        s[3] = seed ^ 0xDAA66D2B;
    }

    // 32bit–³•„†®”‚Ì¶¬
    uint32_t NextUI() {
        const uint32_t result = Rotate(s[0] + s[3], 7) + s[0];
        const uint32_t t = s[1] << 9;

        s[2] ^= s[0];
        s[3] ^= s[1];
        s[1] ^= s[2];
        s[0] ^= s[3];

        s[2] ^= t;
        s[3] = Rotate(s[3], 11);

        return result;
    }

    // 0.0f`1.0f‚Ì³‹K‰»•‚“®¬”“_”‚ğæ“¾
    float GetRandomNormalized() {
        return static_cast<float>(NextUI()) * (1.0f / 4294967295.0f);
    }

  private:
    static inline uint32_t Rotate(const uint32_t x, int k) {
        return (x << k) | (x >> (32 - k));
    }

    uint32_t s[4]; // ó‘Ô•Û—p‚Ì”z—ñ
};

#endif
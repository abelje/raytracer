#pragma once
#include <cstdint>

class Xorshiro256 {
private:
    uint64_t s[4]; // PRNG state

    static inline uint64_t rotl(const uint64_t x, int k);

    uint64_t splitmix64(uint64_t *seed);

    uint64_t next();

    void jump(void);
    void long_jump(void);

public:
    Xorshiro256(uint64_t seed_value = 0) {
        seed(seed_value);
    }

    void seed(uint64_t seed_value);

    double next_double();
};

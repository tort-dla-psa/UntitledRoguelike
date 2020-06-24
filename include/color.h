#pragma once
#include <cstdint>

namespace game{
    struct color{
        using col_t = uint8_t;
        col_t r;
        col_t g;
        col_t b;
        col_t a=255;
    };
};

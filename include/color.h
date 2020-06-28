#pragma once
#include <cstdint>

namespace game{
struct color{
    using col_t = uint8_t;
    col_t r;
    col_t g;
    col_t b;
    col_t a=255;

    color(){};
    color(const color &c){
        *this = c;
    };
    color& operator=(const color &rhs){
        r = rhs.r;
        g = rhs.g;
        b = rhs.b;
        a = rhs.a;
        return *this;
    }

static color from_bytes4(uint8_t c[4]){
    return color::from_bytes(c[0], c[1], c[2], c[3]);
}
static color from_bytes3(uint8_t c[3]){
    return color::from_bytes(c[0], c[1], c[2], 255);
}
static color from_bytes(uint8_t col_r, uint8_t col_g, uint8_t col_b, uint8_t col_a=255) {
    color c;
    c.r = col_r;
    c.g = col_g;
    c.b = col_b;
    c.a = col_a;
    return c;
}
static color from_floats4(float c[4]){
    return color::from_floats(c[0], c[1], c[2], c[3]);
}
static color from_floats3(float c[3]){
    return color::from_floats(c[0], c[1], c[2], 1.f);
}
static color from_floats(float col_r, float col_g, float col_b, float col_a=1){
    return color::from_bytes(col_r*255, col_g*255, col_b*255, col_a*255);
}

};
};

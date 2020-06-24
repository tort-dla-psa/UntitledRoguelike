#include "gfx_funcs.h"
#include <cmath>
#include <iostream>

/*
dot::fp_t gfx_func::distance(const dot &d1, const dot &d2){
    return std::sqrt(d1.x*d2.x + d1.y*d2.y);
}

dot::fp_t gfx_func::length(const dot &d){
    return std::sqrt(d.x*d.x + d.y*d.y);
}

int gfx_func::relation(const dot &d, const dot &beg, const dot &end)
{ return (end.x - beg.x)*(d.y - beg.y) - (end.y - beg.y)*(d.x - beg.x); }

polygon gfx_func::move(const polygon &p, const polygon::dot_t &pos){
    auto p_ = p;
    move_ref(p_, pos);
    return p_;
}
void gfx_func::move_ref(polygon &p, const polygon::dot_t &pos){
    auto dist = pos - p.at(0);
    for(auto& dt:p){
        dt += dist;
    }
}
polygon gfx_func::rotate(const polygon &p, const polygon::fp_t &rad, const polygon::dot_t &pos){
    polygon p_ = p;
    gfx_func::rotate_ref(p_, rad, pos);
    return p_;
}
void gfx_func::rotate_ref(polygon &p, const polygon::fp_t &rad, const polygon::dot_t &pos){
    auto sin = std::sin(rad);
    auto cos = std::cos(rad);
    for(auto& dt:p){
        auto shift = dt-pos;
        auto x = cos*(shift.x) - sin*(shift.y) + pos.x;
        auto y = sin*(shift.x) + cos*(shift.y) + pos.y;
        dt.x = std::move(x);
        dt.y = std::move(y);
    }
}
polygon gfx_func::zoom(const polygon &p, const polygon::fp_t &rad, const polygon::dot_t &pos){
    polygon p_ = p;
    zoom_ref(p_, rad, pos);
    return p_;
}
void gfx_func::zoom_ref(polygon &p, const polygon::fp_t &rad, const polygon::dot_t &pos){
}
*/

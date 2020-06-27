#pragma once
#include <cstddef>

namespace game{

struct dot{
    using coord_t = size_t;
    coord_t x, y, z=0;

    dot()
        :dot(0, 0, 0)
    {}
    dot(const dot &rhs)
        :dot(rhs.x, rhs.y, rhs.z)
    {}
    dot(const coord_t &x, const coord_t &y)
        :x(x),y(y)
    {}

    dot(const coord_t &x, const coord_t &y, const coord_t &z)
        :x(x),y(y),z(z)
    {}

    dot& operator-=(const dot &rhs)
    { return *this = *this-rhs; }

    dot& operator+=(const dot &rhs)
    { return *this = *this+rhs; }

    dot& operator*=(const dot &rhs)
    { return *this = *this*rhs; }

    dot& operator/=(const dot &rhs)
    { return *this = *this/rhs; }

    dot& operator-=(const coord_t &rhs)
    { return *this = *this-dot(rhs,rhs); }

    dot& operator+=(const coord_t &rhs)
    { return *this = *this+dot(rhs,rhs); }

    dot& operator*=(const coord_t &rhs)
    { return *this = *this*dot(rhs,rhs); }

    dot& operator/=(const coord_t &rhs)
    { return *this = *this/dot(rhs,rhs); }

    friend dot operator*(const dot &lhs, const dot &rhs)
    { return dot(lhs.x*rhs.x, lhs.y*rhs.y, lhs.z*rhs.z); }

    friend dot operator/(const dot &lhs, const dot &rhs)
    { return dot(lhs.x/rhs.x, lhs.y/rhs.y, lhs.z/rhs.z); }

    friend dot operator+(const dot &lhs, const dot &rhs)
    { return dot(lhs.x+rhs.x, lhs.y+rhs.y, lhs.z+rhs.z); }

    friend dot operator-(const dot &lhs, const dot &rhs)
    { return dot(lhs.x-rhs.x, lhs.y-rhs.y, lhs.z-rhs.z); }

    friend dot operator+(const dot &lhs, const coord_t &rhs)
    { return lhs + dot(rhs, rhs, rhs); }

    friend dot operator-(const dot &lhs, const coord_t &rhs)
    { return lhs - dot(rhs, rhs, rhs); }

    friend dot operator*(const dot &lhs, const coord_t &rhs)
    { return lhs * dot(rhs, rhs, rhs); }

    friend dot operator/(const dot &lhs, const coord_t &rhs)
    { return lhs / dot(rhs, rhs, rhs); }

    friend bool operator == (const dot &lhs, const dot &rhs){
        return lhs.x == rhs.x &&
            lhs.y == rhs.y &&
            lhs.z == rhs.z;
    }

    friend bool operator < (const dot &lhs, const dot &rhs){
        if(lhs.x < rhs.x) { return true; }
        if(lhs.x > rhs.x) { return false; }
        if(lhs.y < rhs.y) { return true; }
        if(lhs.y > rhs.y) { return false; }
        if(lhs.z < rhs.z) { return true; }
        if(lhs.z > rhs.z) { return false; }
        return false;
    }

    friend bool operator > (const dot &lhs, const dot &rhs)
    { return rhs < lhs; }
};

}

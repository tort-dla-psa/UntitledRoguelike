#pragma once
#include "tile_array.h"

namespace game{

class chunk:public tile_array{
    dim_t m_x, m_y;
public:
    chunk(const dim_t &x, const dim_t &y,
        const dim_t &w=1, const dim_t &h=1, const dim_t &d=1);
    const dim_t& x()const;
    const dim_t& y()const;
    const class tile& get_tile(const dim_t &x, const dim_t &y, const dim_t &z)const;
    void set_tile(const dim_t &x, const dim_t &y, const dim_t &z,
           class tile&& t);
};
chunk::chunk(const chunk::dim_t &x, const chunk::dim_t &y,
    const dim_t &w, const dim_t &h, const dim_t &d)
    :tile_array(w, h, d)
{
    this->m_x = x;
    this->m_y = y;
}
const chunk::dim_t& chunk::x()const
{ return this->m_x; }

const chunk::dim_t& chunk::y()const
{ return this->m_y; }

const tile& chunk::get_tile(const dim_t &x, const dim_t &y, const dim_t &z)const{
    return tile_array::get_tile(x%w(), y%h(), z%d());
}
void chunk::set_tile(const dim_t &x, const dim_t &y, const dim_t &z,
       class tile&& t)
{
    tile_array::set_tile(x%w(), y%h(), z%d(), std::move(t));
}

};

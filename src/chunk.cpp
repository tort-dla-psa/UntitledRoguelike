#include "chunk.h"
using namespace game;

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

const tile& chunk::get_tile(const dim_t &x, const dim_t &y, const dim_t &z)const
{ return tile_array::get_tile(x%w(), y%h(), z%d()); }

tile& chunk::get_tile(const dim_t &x, const dim_t &y, const dim_t &z)
{ return tile_array::get_tile(x%w(), y%h(), z%d()); }

void chunk::set_tile(const dim_t &x, const dim_t &y, const dim_t &z,
       tile&& t)
{
    tile_array::set_tile(x%w(), y%h(), z%d(), std::move(t));
}

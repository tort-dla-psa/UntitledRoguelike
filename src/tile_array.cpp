#include "tile_array.h"
using namespace game;

tile_array::tile_array(const dim_t &w, const dim_t &h, const dim_t &d){
    this->m_w = w;
    this->m_h = h;
    this->m_d = d;
    m_tiles.resize(m_w*m_h*m_d);
}

const tile_array::dim_t& tile_array::w()const
{ return m_w; }

const tile_array::dim_t& tile_array::h()const
{ return m_h; }

const tile_array::dim_t& tile_array::d()const
{ return m_d; }

void tile_array::set_w(const dim_t &dim){
    this->m_w = dim;
    m_tiles.resize(m_w*m_h*m_d);
}

void tile_array::set_h(const dim_t &dim){
    this->m_h = dim;
    m_tiles.resize(m_w*m_h*m_d);
}

void tile_array::set_d(const dim_t &dim){
    this->m_d = dim;
    m_tiles.resize(m_w*m_h*m_d);
}

const tile_array::cont_t& tile_array::tiles()const
{ return m_tiles; }

tile_array::cont_t& tile_array::tiles()
{ return m_tiles; }

const tile& tile_array::get_tile(const dim_t &x, const dim_t &y, const dim_t &z)const
{ return m_tiles.at(x+y*m_w+z*m_w*m_h); }

tile& tile_array::get_tile(const dim_t &x, const dim_t &y, const dim_t &z)
{ return m_tiles.at(x+y*m_w+z*m_w*m_h); }

void tile_array::set_tile(const dim_t &x, const dim_t &y, const dim_t &z, tile&& t)
{ m_tiles.at(x+y*m_w+z*m_w*m_h) = std::move(t); }


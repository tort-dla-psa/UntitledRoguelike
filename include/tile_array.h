#pragma once
#include "tile.h"
#include <vector>

namespace game{

class tile_array{
public:
    using cont_t = std::vector<tile>;
    using dim_t = size_t;
private:
    cont_t m_cont;
    dim_t m_w, m_h, m_d;
public:
    tile_array(const dim_t &w=1, const dim_t &h=1, const dim_t &d=1);
    const dim_t& w()const;
    const dim_t& h()const;
    const dim_t& d()const;
    void set_w(const dim_t &dim);
    void set_h(const dim_t &dim);
    void set_d(const dim_t &dim);
    const class tile& get_tile(const dim_t &x, const dim_t &y, const dim_t &z);
    void set_tile(const dim_t &x, const dim_t &y, const dim_t &z,
           class tile&& t);
};

tile_array::tile_array(const tile_array::dim_t &w, const tile_array::dim_t &h, const tile_array::dim_t &d){
    this->m_w = w;
    this->m_h = h;
    this->m_d = d;
    m_cont.resize(m_w*m_h*m_d);
}
const tile_array::dim_t& tile_array::w()const{ return m_w; }
const tile_array::dim_t& tile_array::h()const{ return m_h; }
const tile_array::dim_t& tile_array::d()const{ return m_d; }
void tile_array::set_w(const tile_array::dim_t &dim){
    this->m_w = dim;
    m_cont.resize(m_w*m_h*m_d);
}
void tile_array::set_h(const tile_array::dim_t &dim){
    this->m_h = dim;
    m_cont.resize(m_w*m_h*m_d);
}
void tile_array::set_d(const tile_array::dim_t &dim){
    this->m_d = dim;
    m_cont.resize(m_w*m_h*m_d);
}
const tile& tile_array::get_tile(const dim_t &x, const dim_t &y, const dim_t &z){
    return m_cont.at(x+y*m_w+z*m_w*m_h);
}
void tile_array::set_tile(const dim_t &x, const dim_t &y, const dim_t &z,
       class tile&& t)
{
    m_cont.at(x+y*m_w+z*m_w*m_h) = std::move(t);
}

}

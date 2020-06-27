#pragma once
#include "tile.h"
#include <vector>

namespace game{

class tile_array{
public:
    using cont_t = std::vector<tile>;
    using dim_t = size_t;
private:
    cont_t m_tiles;
    dim_t m_w, m_h, m_d;
public:
    tile_array(const dim_t &w=1, const dim_t &h=1, const dim_t &d=1);
    const dim_t& w()const;
    const dim_t& h()const;
    const dim_t& d()const;
    void set_w(const dim_t &dim);
    void set_h(const dim_t &dim);
    void set_d(const dim_t &dim);
    const cont_t& tiles()const;
    cont_t& tiles();
    const class tile& get_tile(const dim_t &x, const dim_t &y, const dim_t &z)const;
    void set_tile(const dim_t &x, const dim_t &y, const dim_t &z,
           class tile&& t);
};

}

#pragma once
#include "tile_array.h"

namespace game{

class chunk:public tile_array{
    dim_t m_x, m_y;
public:
    chunk(const dim_t &x, const dim_t &y,
        const dim_t &w, const dim_t &h, const dim_t &d);
    const dim_t& x()const;
    const dim_t& y()const;
    const tile& get_tile(const dim_t &x, const dim_t &y, const dim_t &z)const;
    tile& get_tile(const dim_t &x, const dim_t &y, const dim_t &z);
    void set_tile(const dim_t &x, const dim_t &y, const dim_t &z, tile&& t);
};

}

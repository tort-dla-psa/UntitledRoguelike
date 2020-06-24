#pragma once
#include <algorithm>

namespace game{

class map_obj{
public:
    using coord_t = size_t;
private:
    coord_t m_x,m_y,m_z;
public:
    map_obj();
    const coord_t& x()const;
    const coord_t& y()const;
    const coord_t& z()const;
};
map_obj::map_obj(){}
const map_obj::coord_t& map_obj::x()const{ return m_x; }
const map_obj::coord_t& map_obj::y()const{ return m_y; }
const map_obj::coord_t& map_obj::z()const{ return m_z; }

};

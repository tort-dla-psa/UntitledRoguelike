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
    coord_t& x();
    coord_t& y();
    coord_t& z();
    void set_x(const coord_t &c);
    void set_y(const coord_t &c);
    void set_z(const coord_t &c);
};

};

#include "map_obj.h"

using namespace game;
using coord_t = map_obj::coord_t;

map_obj::map_obj(){}
const coord_t& map_obj::x()const{ return m_x; }
const coord_t& map_obj::y()const{ return m_y; }
const coord_t& map_obj::z()const{ return m_z; }
coord_t& map_obj::x(){ return m_x; }
coord_t& map_obj::y(){ return m_y; }
coord_t& map_obj::z(){ return m_z; }
void map_obj::set_x(const coord_t &c){ m_x = c; }
void map_obj::set_y(const coord_t &c){ m_y = c; }
void map_obj::set_z(const coord_t &c){ m_z = c; }

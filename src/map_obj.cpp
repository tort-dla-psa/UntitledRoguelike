#include "map_obj.h"

using namespace game;
map_obj::map_obj(){}
const map_obj::coord_t& map_obj::x()const{ return m_x; }
const map_obj::coord_t& map_obj::y()const{ return m_y; }
const map_obj::coord_t& map_obj::z()const{ return m_z; }


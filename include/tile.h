#pragma once
#include "materialistic.h"
#include "map_obj.h"

namespace game{

class tile:public Imaterialistic, public map_obj{
public:
    tile();
};

tile::tile(){}

}

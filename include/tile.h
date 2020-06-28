#pragma once
#include "materialistic.h"
#include "map_obj.h"
#include "storing.h"

namespace game{

class tile:public Imaterialistic,
    public Istoring,
    public map_obj
{
public:
    tile();
};
}

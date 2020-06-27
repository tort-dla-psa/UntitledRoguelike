#pragma once
#include "nameable.h"
#include "materialistic.h"
#include "map_obj.h"

namespace game{

class item:public Inameable,
    public Imaterialistic,
    public map_obj
{
public:
    item();
};

item::item()
    :Inameable(),
    Imaterialistic(),
    map_obj()
{}

};

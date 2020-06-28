#pragma once
#include "map_obj.h"
#include "item.h"
#include "nameable.h"

namespace game{

class creature:public item {
public:
    creature(const Inameable::str &name="untitled");
};

};

#pragma once
#include <optional>
#include "nameable.h"
#include "materialistic.h"
#include "map_obj.h"
#include "phys_obj.h"
#include "factory.h"

namespace game{

class item:public Inameable,
    public Imaterialistic,
    public map_obj,
    public phys_obj
{
public:
    item(const material &mat,
        const Inameable::str &name="untitled");
    item(std::shared_ptr<material> mat,
        const Inameable::str &name="untitled");
};

/*
class item_factory:public factory<item>{
    std::optional<Inameable::str> name;
    std::optional<map_obj::coord_t> x, y;
public:
    item_factory();
    void set_name(const Inameable::str &name);
    void set_x(const map_obj::coord_t &x);
    void set_y(const map_obj::coord_t &y);
};
*/

};

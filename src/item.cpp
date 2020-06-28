#include "item.h"

using namespace game;

item::item(const material &mat,
    const Inameable::str &name)
    :Inameable(name),
    Imaterialistic(mat),
    map_obj(),
    phys_obj()
{}

item::item(std::shared_ptr<material> mat,
    const Inameable::str &name)
    :item(*mat, name)
{}

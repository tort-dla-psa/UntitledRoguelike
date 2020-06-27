#include "nameable.h"

using namespace game;
using str = Inameable::str;

Inameable::Inameable(const str &s){
    set_name(s);
}
const Inameable::str& Inameable::name()const{
    return m_name;
}
void Inameable::set_name(const Inameable::str &s){
    this->m_name = s;
}


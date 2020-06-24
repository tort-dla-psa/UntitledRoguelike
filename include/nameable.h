#pragma once
#include <string>

namespace game{

class Inameable{
public:
    using str = std::string;
private:
    str m_name;
public:
    Inameable(const str &s="untitled");
    const str& name()const;
    void set_name(const str &s);
};

Inameable::Inameable(const str &s){
    set_name(s);
}
const Inameable::str& Inameable::name()const{
    return m_name;
}
void Inameable::set_name(const Inameable::str &s){
    this->m_name = s;
}

};

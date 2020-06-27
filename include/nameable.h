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
};

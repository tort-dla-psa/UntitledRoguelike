#pragma once
#include "dot.h"

namespace game{

class phys_obj{
public:
    using dot_t = dot;
    using mass_t = float;
protected:
    dot_t m_pos;
    dot_t m_vel;
    mass_t m_mass;
    bool m_movable;
public:
    phys_obj(const phys_obj &lhs);
    phys_obj(phys_obj &&lhs);
    phys_obj();
    ~phys_obj();

    bool operator==(const phys_obj &lhs)const;
    phys_obj& operator=(const phys_obj &lhs);
    phys_obj& operator=(phys_obj &&lhs);
    bool movable()const;
    void set_movable(bool movable=true);
    const dot_t& vel()const;
    dot_t& vel();
    void set_vel(const dot_t &vel);
    const mass_t& mass()const;
    mass_t& mass();
    void set_mass(const mass_t &mass);
};

}

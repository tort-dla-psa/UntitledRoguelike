#pragma once
#include "nameable.h"
#include "color.h"

namespace game{

class material:public Inameable{
public:
    enum class mat_state{
        plasma,
        gas,
        liquid,
        solid,
    };
    using temp_t = float;
    using color_t = game::color;
private:
    mat_state m_state;
    temp_t m_plasma_to_gas,
        m_gas_to_liquid,
        m_liquid_to_solid;
    color_t m_color;
public:
    material();
    const color_t& color()const;
    void set_color(const color_t& color);
    const mat_state& state()const;
    void set_state(const mat_state &state);
    const temp_t& plasma_to_gas()const;
    const temp_t& gas_to_liquid()const;
    const temp_t& liquid_to_solid()const;
    void set_plasma_to_gas(const temp_t &t);
    void set_gas_to_liquid(const temp_t &t);
    void set_liquid_to_solid(const temp_t &t);
};

material::material(){}
const material::color_t& material::color()const{
    return this->m_color;
}
void material::set_color(const material::color_t& color){
    this->m_color = color;
}
const material::mat_state& material::state()const{
    return m_state;
}
void material::set_state(const material::mat_state &state){
    this->m_state = state;
}
const material::temp_t& material::plasma_to_gas()const{ return m_plasma_to_gas; }
const material::temp_t& material::gas_to_liquid()const{ return m_gas_to_liquid; }
const material::temp_t& material::liquid_to_solid()const{   return m_liquid_to_solid; }
void material::set_plasma_to_gas(const material::temp_t &t){ this->m_plasma_to_gas = t; }
void material::set_gas_to_liquid(const material::temp_t &t){ this->m_gas_to_liquid = t; }
void material::set_liquid_to_solid(const material::temp_t &t){ this->m_liquid_to_solid = t; }

};

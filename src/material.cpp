#include "material.h"

using namespace game;
using color_t = material::color_t;

material::material(){}

std::shared_ptr<const color_t> material::color()const{
    return m_state == mat_state::gas? color_gas():
        m_state == mat_state::liquid? color_liquid():
        color_solid();
}

std::shared_ptr<color_t> material::color(){
    return m_state == mat_state::gas? color_gas():
        m_state == mat_state::liquid? color_liquid():
        color_solid();
}

std::shared_ptr<const color_t> material::color_gas()const
{ return m_color_gas; }

std::shared_ptr<const color_t> material::color_liquid()const 
{ return m_color_liquid; }

std::shared_ptr<const color_t> material::color_solid()const
{ return m_color_solid; }

std::shared_ptr<color_t> material::color_gas()
{ return m_color_gas; }

std::shared_ptr<color_t> material::color_liquid()
{ return m_color_liquid; }

std::shared_ptr<color_t> material::color_solid()
{ return m_color_solid; }

void material::set_color_gas(const color_t& color)
{ set_color_gas(std::make_shared<color_t>(color)); }

void material::set_color_liquid(const color_t& color)
{ set_color_liquid(std::make_shared<color_t>(color)); }

void material::set_color_solid(const color_t& color)
{ set_color_solid(std::make_shared<color_t>(color)); }

void material::set_color_gas(std::shared_ptr<color_t> color)
{ this->m_color_gas = color; }

void material::set_color_liquid(std::shared_ptr<color_t> color)
{ this->m_color_liquid = color; }

void material::set_color_solid(std::shared_ptr<color_t> color)
{ this->m_color_solid = color; }

const material::mat_state& material::state()const
{ return m_state; }

void material::set_state(const material::mat_state &state)
{ this->m_state = state; }

const material::temp_t& material::gas_to_liquid()const
{ return m_gas_to_liquid; }

const material::temp_t& material::liquid_to_solid()const
{ return m_liquid_to_solid; }

void material::set_gas_to_liquid(const material::temp_t &t)
{ this->m_gas_to_liquid = t; }

void material::set_liquid_to_solid(const material::temp_t &t)
{ this->m_liquid_to_solid = t; }

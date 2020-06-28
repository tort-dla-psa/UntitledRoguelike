#include <utility>
#include "phys_obj.h"

using namespace game;
using dot_t = phys_obj::dot_t;
using mass_t = phys_obj::mass_t;

phys_obj::phys_obj() {
    m_movable = true;
}

phys_obj::phys_obj(const phys_obj &lhs)
{ *this = lhs; }

phys_obj::phys_obj(phys_obj &&lhs)
{ *this = std::move(lhs); }

phys_obj::~phys_obj(){}

bool phys_obj::operator==(const phys_obj &lhs)const{
    return m_pos == lhs.m_pos &&
    m_vel      == lhs.m_vel &&
    m_mass     == lhs.m_mass &&
    m_movable  == lhs.m_movable;
}
phys_obj& phys_obj::operator=(const phys_obj &lhs){
    m_pos       = lhs.m_pos;
    m_vel       = lhs.m_vel;
    m_mass      = lhs.m_mass;
    m_movable   = lhs.m_movable;
    return *this;
}
phys_obj& phys_obj::operator=(phys_obj &&lhs){
    m_pos       = std::move(lhs.m_pos);
    m_vel       = std::move(lhs.m_vel);
    m_mass      = std::move(lhs.m_mass);
    m_movable   = std::move(lhs.m_movable);
    return *this;
}

bool phys_obj::movable()const
{ return this->m_movable; }

void phys_obj::set_movable(bool movable)
{ this->m_movable = movable; }

const dot_t& phys_obj::vel()const
{ return this->m_vel; }

dot_t& phys_obj::vel()
{ return this->m_vel; }

void phys_obj::set_vel(const dot_t &vel)
{ this->m_vel = vel; }

const mass_t& phys_obj::mass()const
{ return this->m_mass; }

mass_t& phys_obj::mass()
{ return this->m_mass; }

void phys_obj::set_mass(const mass_t &mass)
{ this->m_mass = mass; }

#include "cam.h"
using namespace game;
using dt_t = cam::dot_t;
using fp_t = cam::fp_t;

cam::cam()
    :m_pos(0,0),
    m_fov(10,10)
{
    m_zoom = 1;
}

const dt_t& cam::pos()const
{ return m_pos; }

void cam::set_pos(const dt_t& pos)
{ this->m_pos = pos; }

const dt_t& cam::fov()const
{ return m_fov; }

void cam::set_fov(const dt_t& fov)
{ this->m_fov = fov; }

const fp_t& cam::zoom()const
{ return m_zoom; }

void cam::set_zoom(const fp_t& zoom)
{ this->m_zoom = zoom; }

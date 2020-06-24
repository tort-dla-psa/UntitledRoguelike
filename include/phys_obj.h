#pragma once
#include "polygon.h"
#include "dot.h"

class phys_obj{
public:
    using dot_t = dot;
    using verts_t = polygon;
    using mass_t = float;
protected:
    dot_t m_pos;
    dot_t m_vel;
    verts_t m_verts;
    mass_t m_mass;
    bool m_movable;
public:
    phys_obj(const verts_t &verts);
    phys_obj(verts_t &&verts);
    phys_obj(const phys_obj &lhs);
    phys_obj(phys_obj &&lhs);
    ~phys_obj();

    bool operator==(const phys_obj &lhs)const;
    phys_obj& operator=(const phys_obj &lhs);
    phys_obj& operator=(phys_obj &&lhs);
    bool movable()const;
    void set_movable(bool movable=true);
    const verts_t& verts()const;
    const dot_t& vert(const size_t &pos)const;
    dot_t& vert(const size_t &pos);
    verts_t& verts();
    void set_verts(const verts_t &verts);
    dot_t mass_center()const;
    const dot_t& vel()const;
    void set_vel(const dot_t &vel);
    const mass_t& mass()const;
    void set_mass(const mass_t &mass);
};


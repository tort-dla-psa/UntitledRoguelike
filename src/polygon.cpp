#include "polygon.h"
#include "gfx_funcs.h"
#include <algorithm>
#include <limits>

/*
using dot_t = polygon::dot_t;
using verts_t = polygon::verts_t;
using fp_t = polygon::fp_t;

polygon::polygon(const verts_t &verts)
    :m_verts(verts)
{}

polygon::polygon(verts_t &&verts)
    :m_verts(std::move(verts))
{}

polygon::polygon(const polygon &lhs)
{ this->m_verts = lhs.m_verts; }

polygon::polygon(polygon &&lhs)
{ this->m_verts = std::move(lhs.m_verts); }


bool polygon::operator==(const polygon &lhs)const
{ return m_verts == lhs.m_verts; }

polygon& polygon::operator=(const verts_t &verts){
    this->m_verts = verts;
    return *this;
}
polygon& polygon::operator=(verts_t &&verts){
    this->m_verts = std::move(verts);
    return *this;
}
polygon& polygon::operator=(const polygon &lhs){
    this->m_verts = lhs.m_verts;
    return *this;
}
polygon& polygon::operator=(polygon &&lhs){
    this->m_verts = std::move(lhs.m_verts);
    return *this;
}

bool polygon::contains(const dot_t &dot)const{
    auto testx = dot.x;
    auto testy = dot.y;
    auto nvert = m_verts.size();
    int wn = 0;    // the  winding number counter

    // loop through all edges of the polygon
    for (int i=0; i<nvert; i++) {       // edge from V[i] to  V[i+1]
        auto vert = m_verts[i];
        auto next = m_verts[(i+1)%nvert];
        if (vert.y <= dot.y) {          // start y <= P.y
            if (next.y > dot.y)         // an upward crossing
                if (gfx_func::relation(dot, vert, next) > 0)    // P left of  edge
                     ++wn;              // have  a valid up intersect
        } else {                        // start y > P.y (no test needed)
            if (next.y <= dot.y)        // a downward crossing
                 if (gfx_func::relation(dot, vert, next) < 0)    // P right of  edge
                     --wn;              // have  a valid down intersect
        }
    }
    return wn!=0;
}

verts_t& polygon::verts(const verts_t &verts)
{ return this->m_verts; }

const verts_t& polygon::verts(const verts_t &verts)const
{ return this->m_verts; }

dot_t& polygon::add_vert(const dot_t &d){
    return m_verts.emplace_back(d);
}

dot_t& polygon::add_vert(const dot_t &d, const size_t &pos){
    return *m_verts.emplace(std::next(m_verts.begin(), pos), d);
}

dot_t& polygon::del_vert(const dot_t &d){
    auto it = std::find(m_verts.begin(), m_verts.end(), d);
    auto next_it = m_verts.erase(it);
    return *next_it;
}

verts_t::iterator polygon::begin()
{ return m_verts.begin(); }

verts_t::iterator polygon::end()
{ return m_verts.end(); }

verts_t::const_iterator polygon::begin()const
{ return m_verts.cbegin(); }

verts_t::const_iterator polygon::end()const
{ return m_verts.cend(); }

const size_t polygon::size()const
{ return m_verts.size(); }

const dot_t& polygon::at(const size_t &pos)const
{ return m_verts.at(pos); }

dot_t& polygon::at(const size_t &pos)
{ return m_verts.at(pos); }

std::vector<dot_t> polygon::normals()const{
    std::vector<dot_t> result;
    result.reserve(m_verts.size());
    for(size_t i=0; i<m_verts.size(); i++){
        auto cur = m_verts.at(i);
        auto next = m_verts.at((i+1)%m_verts.size());
        auto edge = std::move(next) - std::move(cur);
        auto norm = dot{-edge.y, edge.x};
        result.emplace_back(std::move(norm));
    }
    return result;
}

fp_t polygon::signed_area()const{
    fp_t result=0;
    auto size = m_verts.size();
    for(size_t i=0; i<size; i++){
        auto pt = m_verts[i];
        auto pt_next = m_verts[(i+1)%size];
        result += (pt.x * pt_next.y) - (pt_next.x * pt.y);
    }
    result /= 2;
    return result;
}

dot_t polygon::centroid()const{
    dot_t result = {0, 0};
    fp_t signed_area_x2 = 0;
    auto size = m_verts.size();
    for(size_t i=0; i<size; i++){
        auto pt = m_verts[i];
        auto pt_next = m_verts[(i+1)%size];
        auto tmp = (pt.x * pt_next.y) - (pt_next.x * pt.y);
        signed_area_x2 += tmp;
        result.x += (pt.x + pt_next.x)*tmp;
        result.y += (pt.y + pt_next.y)*tmp;
    }
    result /= signed_area_x2*3;
    return result;
}
*/

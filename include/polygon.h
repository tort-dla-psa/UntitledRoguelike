#pragma once
#include <vector>
#include <initializer_list>
#include "dot.h"

/*
class polygon{
public:
    using dot_t = dot;
    using verts_t = std::vector<dot_t>;
    using fp_t = float;
private:
    verts_t m_verts;
public:
    polygon(const verts_t &verts={});
    polygon(verts_t &&verts);
    polygon(const polygon &lhs);
    polygon(polygon &&lhs);

    bool operator==(const polygon &lhs)const;
    polygon& operator=(const verts_t &verts);
    polygon& operator=(verts_t &&verts);
    polygon& operator=(const polygon &lhs);
    polygon& operator=(polygon &&lhs);

    bool contains(const dot_t &dot)const;
    verts_t::iterator begin();
    verts_t::iterator end();
    verts_t::const_iterator begin()const;
    verts_t::const_iterator end()const;
    const size_t size()const;
    const dot_t& at(const size_t &pos)const;
    dot_t& at(const size_t &pos);
    verts_t& verts(const verts_t &verts);
    const verts_t& verts(const verts_t &verts)const;
    dot_t& add_vert(const dot_t &d, const size_t &pos);
    dot_t& add_vert(const dot_t &d);
    dot_t& del_vert(const dot_t &d);
    std::vector<dot_t> normals()const;
    fp_t signed_area()const;
    dot_t centroid()const;
};
*/

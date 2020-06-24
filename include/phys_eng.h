#pragma once
#include <tuple>
#include <vector>
#include "phys_obj.h"

class phys_eng{
public:
    using g_t = float;
private:
    g_t m_g;
public:
    phys_eng(const g_t = 9.81);
    const g_t& g()const;
    void set_g(const g_t &g);
    void process(phys_obj &obj, const std::vector<phys_obj> &objs);

    std::vector<std::tuple<size_t, dot::fp_t, dot::fp_t>> //id, a-diff, b-diff
        find_collisions (phys_obj &obj, const std::vector<phys_obj> &objs);

    void move(phys_obj &obj, const phys_obj::dot_t &pos);
};

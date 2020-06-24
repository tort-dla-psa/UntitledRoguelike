#include "phys_eng.h"
#include "gfx_funcs.h"
#include <limits>

phys_eng::phys_eng(const phys_eng::g_t g)
    :m_g(g)
{}
const phys_eng::g_t& phys_eng::g()const
{ return m_g; }

void phys_eng::set_g(const phys_eng::g_t &g)
{ this->m_g = g; }

std::vector<std::tuple<size_t, dot::fp_t, dot::fp_t>> //id, x-diff, y-diff
    phys_eng::find_collisions(phys_obj &obj, const std::vector<phys_obj> &objs)
{
    std::vector<std::tuple<size_t, dot::fp_t, dot::fp_t>> result;
    auto& verts = obj.verts();
    auto norms = verts.normals();

    for(size_t i=0; i<norms.size(); i++){
        auto norm = norms.at(i);
        auto a_max_proj = -std::numeric_limits<dot::fp_t>::infinity();
        auto a_min_proj = std::numeric_limits<dot::fp_t>::infinity();
        for(auto& a_v:verts){
            auto proj = norm.x*a_v.x+norm.y*a_v.y;
			if(proj < a_min_proj) a_max_proj = proj;
			if(proj > a_max_proj) a_max_proj = proj;
        }
        for(auto &o:objs){
            auto b_max_proj = -std::numeric_limits<dot::fp_t>::infinity();
            auto b_min_proj = std::numeric_limits<dot::fp_t>::infinity();
            auto verts = o.verts();
            for(auto& b_v:verts){
                auto proj = norm.x*b_v.x+norm.y*b_v.y;
                if(proj < b_min_proj) b_max_proj = proj;
                if(proj > b_max_proj) b_max_proj = proj;
            }
            if(a_max_proj < b_min_proj){
                auto diff = b_min_proj - a_max_proj;
                auto x_dist = 0;
            }
            if(a_min_proj > b_max_proj) {
                auto diff = b_max_proj - a_min_proj;
                auto x_dist = 0;
            }
        }
    }
    return result;
}

void phys_eng::process(phys_obj &obj, const std::vector<phys_obj> &objs) {
    auto vel = obj.vel();
    auto mass = obj.mass();
    auto mnt = vel*mass;
    auto& verts = obj.verts();
    auto norms = verts.normals();
    //
    //check collisions
    auto collided_ids = find_collisions(obj, objs);
    for(auto &tpl:collided_ids){
        auto id = std::get<0>(tpl);
        auto a_diff = std::get<1>(tpl);
        auto b_diff = std::get<2>(tpl);
        auto col_obj = objs.at(id);
    }
    //change velocity with gravity
    for(auto &o:objs){
    }
}

void phys_eng::move(phys_obj &obj, const phys_obj::dot_t &pos){
    gfx_func::move_ref(obj.verts(), pos);
}

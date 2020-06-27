#pragma once
#include "tile.h"
#include "map.h"
#include "material.h"
#include "color.h"
#include "map.h"
#include "game.h"
#include "nlohmann-json/single_include/nlohmann/json.hpp"

namespace game{

void to_json(nlohmann::json& j, const color &c) {
	j["r"] = c.r;
	j["g"] = c.g;
	j["b"] = c.b;
	j["a"] = c.a;
}

void to_json(nlohmann::json& j, const Inameable &n) {
	j["name"] = n.name();
}

void to_json(nlohmann::json& j, const material &m) {
	j["t_gas_to_liquid"] = m.gas_to_liquid();
    j["t_liquid_to_solid"] = m.liquid_to_solid();
	j["color_gas"] = *m.color_gas();
	j["color_liquid"] = *m.color_liquid();
	j["color_solid"] = *m.color_solid();
    const Inameable &cast = m;
    to_json(j, cast);
}

void to_json(nlohmann::json& j, const tile& t) {
    j["x"] = t.x();
    j["y"] = t.y();
    j["z"] = t.z();
	j["material"] = t.mat();
}

void to_json(nlohmann::json& j, const tile_array& t) {
    j["w"] = t.w();
    j["h"] = t.h();
    j["d"] = t.d();
    j["tiles"] = t.tiles();
}

void to_json(nlohmann::json& j, const chunk& c) {
    j["x"] = c.x();
    j["y"] = c.y();
    const tile_array &cast = c;
    to_json(j, cast);
}

void to_json(nlohmann::json& j, const map& m) {
    /*
    auto ch_cont = m.chunks(); //container with chunks, std::map
    std::map<std::string, chunk> json_map;
    for(auto &pair:ch_cont){
        auto coord = pair.first;
        auto coord_str = std::to_string(coord.x)+","+
            std::to_string(coord.y);
        json_map.emplace(coord_str, *pair.second);
    }
    j["chunks"] = std::move(json_map);
    */
    auto settings = m.settings();
    j["seed_height"] = settings.seed_heights;
    j["seed_caves"] =  settings.seed_caves;
    j["chunk_w"] = settings.chunk_w;
    j["chunk_h"] = settings.chunk_h;
    j["chunk_d"] = settings.chunk_d;
    j["materials"] = settings.materials;
}

void to_json(nlohmann::json& j, const engine& e) {
    j["map"] = e.get_map();
}

void from_json(const nlohmann::json& j, Inameable &n) {
    std::string name;
	j.at("name").get_to(name);
    n.set_name(name);
}

void from_json(const nlohmann::json& j, color &c) {
	j.at("r").get_to(c.r);
	j.at("g").get_to(c.g);
	j.at("b").get_to(c.b);
	j.at("a").get_to(c.a);
}

void from_json(const nlohmann::json& j, material &m) {
    game::color color_gas, color_liquid, color_solid;
    game::material::temp_t gas_to_liquid, liquid_to_solid;
	j.at("t_gas_to_liquid").get_to(gas_to_liquid);
    j.at("t_liquid_to_solid").get_to(liquid_to_solid);
	j.at("color_gas").get_to(color_gas);
	j.at("color_liquid").get_to(color_liquid);
	j.at("color_solid").get_to(color_solid);
    m.set_color_gas(color_gas);
    m.set_color_liquid(color_liquid);
    m.set_color_solid(color_solid);
    m.set_gas_to_liquid(gas_to_liquid);
    m.set_liquid_to_solid(liquid_to_solid);
    Inameable &cast = m;
    from_json(j, cast);
}

void from_json(const nlohmann::json &j, map &m){
    map_gen_settings settings;
    j.at("chunk_w").get_to(settings.chunk_w);
    j.at("chunk_h").get_to(settings.chunk_h);
    j.at("chunk_d").get_to(settings.chunk_d);
    j.at("seed_height").get_to(settings.seed_heights);
    j.at("seed_caves").get_to(settings.seed_caves);
    j.at("materials").get_to(settings.materials);
    m = game::map(settings);
}

void from_json(const nlohmann::json &j, engine &e){
    j.at("map").get_to(e.get_map());
}

};

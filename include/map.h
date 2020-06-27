#pragma once
#include <memory>
#include <map>
#include "tile.h"
#include "dot.h"
#include "chunk.h"
#include "FastNoise.h"

namespace game{

struct map_gen_settings{
    std::vector<material> materials;
    size_t seed_heights, seed_caves;
    tile_array::dim_t chunk_w, chunk_h, chunk_d;

    map_gen_settings(){
        seed_heights = 0;
        seed_caves = 1;
        chunk_w = 32;
        chunk_h = 32;
        chunk_d = 128;

        game::material air, gnd;
        game::color air_clr{ 200, 245, 255, 40 };
        air.set_name("air");
        air.set_color_gas(air_clr);
        air.set_color_liquid(air_clr);
        air.set_color_solid(air_clr);

        game::color gnd_clr{   0,  30,   7, 255 };
        gnd.set_name("dirt");
        gnd.set_color_gas(gnd_clr);
        gnd.set_color_liquid(gnd_clr);
        gnd.set_color_solid(gnd_clr);
        materials.emplace_back(std::move(air));
        materials.emplace_back(std::move(gnd));
    }
};

class map{
public:
    template<class T>
    using ptr_t = std::shared_ptr<T>;
    using chunk_cont_t = std::map<dot, ptr_t<chunk>>;
private:
    chunk_cont_t m_chunks;
    map_gen_settings m_settings;
    FastNoise m_height_gen,
              m_cave_gen,
              m_biome_gen;
public:
    map();
    map(const map_gen_settings &settings);
    const map_gen_settings& settings()const;
    void set_gen_biome(const FastNoise &gen);
    void set_gen_height(const FastNoise &gen);
    void set_gen_cave(const FastNoise &gen);
    const FastNoise& gen_biome()const;
    const FastNoise& gen_height()const;
    const FastNoise& gen_cave()const;

    const chunk_cont_t& chunks()const;
    chunk_cont_t& chunks();
    ptr_t<chunk> get_chunk(const dot &place);
    ptr_t<chunk> get_chunk(const size_t &x, const size_t &y);
    ptr_t<chunk> gen_chunk(const dot &place);
    ptr_t<chunk> gen_chunk(const size_t &x, const size_t &y);
    void del_chunk(const dot &place);
    void del_chunk(const size_t &x, const size_t &y);
};

}

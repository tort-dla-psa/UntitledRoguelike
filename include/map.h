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

map::map()
    :map(m_settings)
{}

map::map(const map_gen_settings &settings) {
    this->m_settings = settings;

    m_height_gen.SetSeed(m_settings.seed_heights);
    m_height_gen.SetSeed(m_settings.seed_caves);

    m_height_gen.SetNoiseType(FastNoise::SimplexFractal);
    m_height_gen.SetNoiseType(FastNoise::SimplexFractal);
}

const map_gen_settings& map::settings()const
{ return m_settings; }

void map::set_gen_biome(const FastNoise &gen)
{ this->m_biome_gen = gen; }

void map::set_gen_height(const FastNoise &gen)
{ this->m_height_gen = gen; }

void map::set_gen_cave(const FastNoise &gen)
{ this->m_cave_gen = gen; }

const FastNoise& map::gen_biome()const
{ return this->m_biome_gen; }

const FastNoise& map::gen_height()const
{ return this->m_height_gen; }

const FastNoise& map::gen_cave()const
{ return this->m_cave_gen; }

const map::chunk_cont_t& map::chunks()const
{ return m_chunks; }

map::chunk_cont_t& map::chunks()
{ return m_chunks; }

map::ptr_t<chunk> map::get_chunk(const dot &place)
{ return get_chunk(place.x, place.y); } 

map::ptr_t<chunk> map::get_chunk(const size_t &x, const size_t &y){
    auto &ch_w = m_settings.chunk_w;
    auto &ch_h = m_settings.chunk_h;
    auto beg_x = x/ch_w*ch_w;
    auto beg_y = y/ch_h*ch_h;
    dot d = dot(beg_x, beg_y);
    auto it = m_chunks.find(d);
    return (it == m_chunks.end())?nullptr:it->second;
}

map::ptr_t<chunk> map::gen_chunk(const dot &place)
{ return gen_chunk(place.x, place.y); }

map::ptr_t<chunk> map::gen_chunk(const size_t &x, const size_t &y){
    using dim_t = tile_array::dim_t;
    const auto& mats = m_settings.materials;
    const auto air_it = std::find_if(mats.begin(), mats.end(),
           [](const auto &mat) {
               return mat.name() == "air";
           });
    const auto gnd_it = std::find_if(mats.begin(), mats.end(),
           [](const auto &mat) {
               return mat.name() == "dirt";
           });
    const auto air = *air_it;
    const auto gnd = *gnd_it;
    auto &ch_w = m_settings.chunk_w;
    auto &ch_h = m_settings.chunk_h;
    auto &ch_d = m_settings.chunk_d;
    auto beg_x = x/ch_w*ch_w;
    auto beg_y = y/ch_h*ch_h;

    float cave_min = 0.9;
    float cave_max = 1.0;
    std::vector<float> heightMap;
    heightMap.resize(ch_w*ch_h);

    auto ch = std::make_shared<chunk>(beg_x, beg_y, ch_w, ch_h, ch_d);
    for(dim_t x = beg_x; x < beg_x+ch_w; x++) {
        for(dim_t y = beg_y; y < beg_y+ch_h; y++) {
            auto noise = m_height_gen.GetNoise(x, y);
            auto val = ch_d*0.5*(1.0+noise);
            heightMap.at((x%ch_w) + (y%ch_h)*ch_w) = std::move(val);
            for(dim_t z = 0; z < dim_t(val); z++) {
                tile t;
                t.set_mat(air);
                ch->set_tile(x,y,z, std::move(t));
            }
            for(dim_t z = dim_t(val); z < ch_d; z++) {
                auto cave_val = m_cave_gen.GetNoise(x, y, z);
                tile t;
                if(cave_val >= cave_min &&
                        cave_val < cave_max)
                {
                    t.set_mat(air);
                }else{
                    t.set_mat(gnd);
                }
                ch->set_tile(x,y,z, std::move(t));
            }
        }
    }
    dot d = dot(beg_x, beg_y);
    m_chunks[d] = ch;
    return ch;
}

void map::del_chunk(const dot &place)
{ del_chunk(place.x, place.y); }

void map::del_chunk(const size_t &x, const size_t &y) {
    auto &ch_w = m_settings.chunk_w;
    auto &ch_h = m_settings.chunk_h;
    auto beg_x = x/ch_w*ch_w;
    auto beg_y = y/ch_h*ch_h;
    auto it = m_chunks.find(dot(beg_x, beg_y));
    if(it == m_chunks.end())
    { return; }
    m_chunks.erase(it);
}

}

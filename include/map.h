#pragma once
#include <memory>
#include <map>
#include "tile.h"
#include "dot.h"
#include "chunk.h"
#include "FastNoise.h"

namespace game{

struct map_gen_settings{
    material air, solid;
    tile_array::dim_t chunk_w, chunk_h, chunk_d;
};

class map{
public:
    template<class T>
    using ptr_t = std::shared_ptr<T>;
private:
    std::map<dot, ptr_t<chunk>> m_chunks;
    map_gen_settings m_settings;
    FastNoise m_height_gen,
              m_cave_gen,
              m_biome_gen;
public:
    map(const map_gen_settings &settings);
    void set_biome_gen(const FastNoise &gen);
    void set_height_gen(const FastNoise &gen);
    void set_cave_gen(const FastNoise &gen);
    const FastNoise& biome_gen()const;
    const FastNoise& height_gen()const;
    const FastNoise& cave_gen()const;

    ptr_t<chunk> get_chunk(const dot &place);
    ptr_t<chunk> get_chunk(const size_t &x, const size_t &y);
    ptr_t<chunk> gen_chunk(const dot &place);
    ptr_t<chunk> gen_chunk(const size_t &x, const size_t &y);
    void del_chunk(const dot &place);
    void del_chunk(const size_t &x, const size_t &y);
};

map::map(const map_gen_settings &settings) {
    this->m_settings = settings;
    m_height_gen.SetSeed(0);
    m_height_gen.SetNoiseType(FastNoise::SimplexFractal);
}

void map::set_biome_gen(const FastNoise &gen)
{ this->m_biome_gen = gen; }

void map::set_height_gen(const FastNoise &gen)
{ this->m_height_gen = gen; }

void map::set_cave_gen(const FastNoise &gen)
{ this->m_cave_gen = gen; }

const FastNoise& map::biome_gen()const
{ return this->m_biome_gen; }

const FastNoise& map::height_gen()const
{ return this->m_height_gen; }

const FastNoise& map::cave_gen()const
{ return this->m_cave_gen; }

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
    auto &air = m_settings.air;
    auto &solid = m_settings.solid;
    auto &ch_w = m_settings.chunk_w;
    auto &ch_h = m_settings.chunk_h;
    auto &ch_d = m_settings.chunk_d;
    auto beg_x = x/ch_w*ch_w;
    auto beg_y = y/ch_h*ch_h;

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
                tile t;
                t.set_mat(solid);
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

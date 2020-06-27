#include "map.h"

using namespace game;

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

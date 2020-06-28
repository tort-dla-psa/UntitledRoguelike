#include "game.h"
#include "json_io.h"

using namespace game;
using items_cont = engine::items_cont;

engine::engine()
    :m_sem(4)
{}

engine::~engine(){ }

void engine::tick(){
}

void engine::set_map(const class map &m)
{ this->m_map = m; }

const map& engine::get_map()const
{ return m_map; }

map& engine::get_map()
{ return m_map; }

std::future<map::ptr_t<chunk>>
    engine::get_chunk(const chunk::dim_t &x, const chunk::dim_t &y){
    auto task = [this](auto x, auto y){
        semaphore_controller sc(this->m_sem);
        auto ch = m_map.get_chunk(x, y);
        if(!ch){
            ch = m_map.gen_chunk(x, y);
        }
        return ch;
    };
    return std::async(std::launch::async, std::move(task), x, y);
}

bool engine::add_item(const item &i){
    m_items.emplace_back(std::make_shared<item>(i));
    auto ch_ptr = m_map.get_chunk(i.x(), i.y());
    auto &ch = *ch_ptr;
    auto &tl = ch.get_tile(i.x(), i.y(), i.z());
    const auto &tl_mat = tl.mat();
    tl.emplace(i);
    return true;
}
items_cont& engine::items(){ return m_items; }
const items_cont& engine::items()const{ return m_items; }

void engine::save(const std::string &path)const{
    nlohmann::json game_json = m_map;
    std::ofstream ofs(path);
    ofs << game_json;
    ofs.close();
}

void engine::load(const std::string &path){
    nlohmann::json game_json;
    std::ifstream ifs(path);
    ifs >> game_json;
    ifs.close();
    m_map = game_json.get<game::map>();
}


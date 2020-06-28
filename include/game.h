#pragma once
#include <memory>
#include <thread>
#include <future>
#include <mutex>
#include <fstream>
#include <iomanip>
#include "rwqueue/readerwriterqueue.h"
#include "map.h"
#include "chunk.h"
#include "item.h"

namespace game{

class semaphore {
private:
    std::mutex m;
    std::condition_variable cv;
    unsigned int count;
public:
    semaphore(const unsigned int &n)
        :count(n)
    {}
    void notify() {
        std::unique_lock<std::mutex> l(m);
        ++count;
        cv.notify_one();
    }
    void wait() {
        std::unique_lock<std::mutex> l(m);
        cv.wait(l, [this]{ return count != 0; });
        --count;
    }
};

class semaphore_controller {
    semaphore &s;
public:
    semaphore_controller(semaphore &s)
        :s{s}
    { s.wait(); }
    ~semaphore_controller()
    { s.notify(); }
};

class engine{
public:
    using items_cont = std::vector<std::shared_ptr<item>>;
private:
    map m_map;
    semaphore m_sem;
    items_cont m_items;
public:
    engine();
    ~engine();
    void tick();
    void set_map(const class map &m);
    const map& get_map()const;
    map& get_map();
    std::future<map::ptr_t<chunk>>
        get_chunk(const chunk::dim_t &x, const chunk::dim_t &y);
    bool add_item(const item &i);
    items_cont& items();
    const items_cont& items()const;
    void save(const std::string &path)const;
    void load(const std::string &path);
};

};

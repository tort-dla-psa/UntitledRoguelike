#pragma once
#include <vector>
#include <memory>
#include "item.h"

namespace game{
class Istoring{
public:
    using cont_t = std::vector<std::shared_ptr<item>>;
    using val_t = cont_t::value_type;
    using it_t = cont_t::iterator;
    using c_it_t = cont_t::const_iterator;
private:
    cont_t m_items;
public:
    Istoring();
    Istoring(const size_t &size);
    it_t begin();
    it_t end();
    c_it_t begin()const;
    c_it_t end()const;
    void reserve(const size_t &size);
    void resize(const size_t &size);
    void remove(const size_t &i);
    val_t& at(const size_t &i);
    const val_t& at(const size_t &i)const;
    size_t size()const;
    void emplace(const item &i);
    void emplace(const val_t &val);
    void emplace(item &&i);
    void emplace(val_t &&val);
    void emplace(const item &i, const size_t &place);
    void emplace(item &&i, const size_t &place);
    void emplace(const val_t &val, const size_t &place);
    void emplace(val_t &&val, const size_t &place);
};
};

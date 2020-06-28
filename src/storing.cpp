#include "storing.h"

using namespace game;
using cont_t = Istoring::cont_t;
using val_t = Istoring::val_t;
using it_t = Istoring::it_t;
using c_it_t = Istoring::c_it_t;

Istoring::Istoring(){}
Istoring::Istoring(const size_t &size)
{ resize(size); }

it_t Istoring::begin(){ return m_items.begin(); }
it_t Istoring::end()  { return m_items.end(); };
c_it_t Istoring::begin()const{ return m_items.cbegin(); }
c_it_t Istoring::end()const  { return m_items.cend(); }

void Istoring::reserve(const size_t &size)
{ m_items.reserve(size); }

void Istoring::resize(const size_t &size)
{ m_items.resize(size); }

void Istoring::remove(const size_t &i)
{ m_items.erase(std::next(m_items.begin(), i)); }

val_t& Istoring::at(const size_t &i)
{ return m_items.at(i); }

const val_t& Istoring::at(const size_t &i)const
{ return m_items.at(i); }

size_t Istoring::size()const
{ return m_items.size(); }

void Istoring::emplace(const item &i)
{ m_items.emplace_back(std::make_shared<item>(i)); }

void Istoring::emplace(const val_t &val)
{ m_items.emplace_back(val); }

void Istoring::emplace(item &&i)
{ m_items.emplace_back(std::make_shared<item>(std::move(i))); }

void Istoring::emplace(val_t &&val)
{ m_items.emplace_back(std::move(val)); }

void Istoring::emplace(const item &i, const size_t &place){
    auto it = std::next(m_items.begin(), place);
    m_items.emplace(it, std::make_shared<item>(i));
}

void Istoring::emplace(const val_t &val, const size_t &place){
    auto it = std::next(m_items.begin(), place);
    m_items.emplace(it, val);
}

void Istoring::emplace(item &&i, const size_t &place){
    auto it = std::next(m_items.begin(), place);
    m_items.emplace(it, std::make_shared<item>(std::move(i)));
}

void Istoring::emplace(val_t &&val, const size_t &place){
    auto it = std::next(m_items.begin(), place);
    m_items.emplace(it, std::move(val));
}

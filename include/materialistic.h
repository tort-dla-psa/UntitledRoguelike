#pragma once
#include <memory>
#include "material.h"

namespace game{

class Imaterialistic{
    std::shared_ptr<material> m_mat; //to minimize derived classes
public:
    Imaterialistic();
    const material& mat()const;
    material& mat();
    void set_mat(const material &mat);
    std::shared_ptr<const material> mat_ptr()const;
    std::shared_ptr<material> mat_ptr();
    void set_mat(std::shared_ptr<material> mat);
};

Imaterialistic::Imaterialistic(){}

const material& Imaterialistic::mat()const
{ return *m_mat; }

material& Imaterialistic::mat()
{ return *m_mat; }

void Imaterialistic::set_mat(const material &mat)
{ this->m_mat = std::make_shared<material>(mat); }

std::shared_ptr<const material> Imaterialistic::mat_ptr()const
{ return this->m_mat; }

std::shared_ptr<material> Imaterialistic::mat_ptr()
{ return this->m_mat; }

void Imaterialistic::set_mat(std::shared_ptr<material> mat)
{ this->m_mat = mat; }

};

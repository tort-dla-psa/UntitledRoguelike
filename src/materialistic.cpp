#include "materialistic.h"

using namespace game;

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

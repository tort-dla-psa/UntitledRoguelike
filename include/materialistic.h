#pragma once
#include <memory>
#include "material.h"

namespace game{

class Imaterialistic{
    std::shared_ptr<material> m_mat; //to minimize derived classes
public:
    Imaterialistic();
    const material& mat()const;
    void set_mat(const material &mat);
};

Imaterialistic::Imaterialistic(){}
const material& Imaterialistic::mat()const{
    return *m_mat;
}
void Imaterialistic::set_mat(const material &mat){
    this->m_mat = std::make_shared<material>(mat);
}

};

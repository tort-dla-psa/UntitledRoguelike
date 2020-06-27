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

};

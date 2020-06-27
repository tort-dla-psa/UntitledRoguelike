#pragma once
#include <memory>
#include "nameable.h"
#include "color.h"

namespace game{

class material:public Inameable{
public:
    enum class mat_state{
        gas,
        liquid,
        solid,
    };
    using temp_t = float;
    using color_t = game::color;
private:
    mat_state m_state;
    temp_t m_gas_to_liquid,
        m_liquid_to_solid;
    std::shared_ptr<color_t> m_color_gas,
        m_color_liquid,
        m_color_solid;
public:
    material();
    std::shared_ptr<const color_t> color()const;
    std::shared_ptr<const color_t> color_gas()const;
    std::shared_ptr<const color_t> color_liquid()const;
    std::shared_ptr<const color_t> color_solid()const;
    std::shared_ptr<color_t> color();
    std::shared_ptr<color_t> color_gas();
    std::shared_ptr<color_t> color_liquid();
    std::shared_ptr<color_t> color_solid();
    void set_color_gas(const color_t& color);
    void set_color_liquid(const color_t& color);
    void set_color_solid(const color_t& color);
    void set_color_gas(std::shared_ptr<color_t> color);
    void set_color_liquid(std::shared_ptr<color_t> color);
    void set_color_solid(std::shared_ptr<color_t> color);
    const mat_state& state()const;
    void set_state(const mat_state &state);
    const temp_t& gas_to_liquid()const;
    const temp_t& liquid_to_solid()const;
    void set_gas_to_liquid(const temp_t &t);
    void set_liquid_to_solid(const temp_t &t);
};

};

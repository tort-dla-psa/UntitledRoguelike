#pragma once
#include <memory>

namespace game{
template<class T>
class factory{
protected:
    std::unique_ptr<T> m_t;
public:
    inline factory(){}
    inline T get(){
        T t = *m_t.get();
        return t;
    }
};
}

#pragma once
#include "rwqueue/readerwriterqueue.h"

namespace game{

class engine{
public:
    using queue = moodycamel::BlockingReaderWriterQueue<int>;
private:
    queue m_q;
public:
    engine(const queue &q);
    void run();
    void stop();
};

engine::engine(const queue &q){}
void engine::run(){}
void engine::stop(){}

};

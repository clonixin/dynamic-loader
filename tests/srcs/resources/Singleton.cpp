#include "./Singleton.hpp"

namespace tests::resources {
    Singleton &Singleton::getInstance(int init_v) {
        static Singleton s(init_v);

        return s;
    }

    int Singleton::getVal() const {
        return _val;
    }
}

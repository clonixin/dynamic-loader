#ifndef RES_MOVABLE_HPP__
#define RES_MOVABLE_HPP__

#include "./globals.h"

namespace tests::resources {
    class Movable {
        public:
            Movable(int v): _val(v) {}
            Movable(Movable const &c) = delete;
            Movable(Movable &&m): _val(m._val) {
                tests::glob::moved = true;
            }
        private:
            int _val;
    };
}

#endif

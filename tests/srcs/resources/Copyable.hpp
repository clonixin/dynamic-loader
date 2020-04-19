#ifndef RES_COPYABLE_HPP__
#define RES_COPYABLE_HPP__

#include "./globals.h"

namespace tests::resources {
    class Copyable {
        public:
            Copyable(int v): _val(v) {}
            Copyable(Copyable const &c): _val(c._val) {
                tests::glob::copied = true;
            }
            Copyable(Copyable &&) = delete;
        private:
            int _val;
    };
}

#endif

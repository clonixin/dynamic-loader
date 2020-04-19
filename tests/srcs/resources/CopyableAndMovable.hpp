#ifndef RES_COPYABLEANDMOVABLE_HPP__
#define RES_COPYABLEANDMOVABLE_HPP__

#include "./globals.h"

namespace tests::resources {
    class CopyableAndMovable {
        public:
            CopyableAndMovable(int v): _val(v) {}
            CopyableAndMovable(CopyableAndMovable const &b): _val(b._val) {
                glob::copied = true;
            }
            CopyableAndMovable(CopyableAndMovable &&b): _val(b._val) {
                glob::moved = true;
            }
        private:
            int _val;
    };
}

#endif

#ifndef RES_GLOBALS_HPP__
#define RES_GLOBALS_HPP__
#include <iostream>

namespace tests {
    namespace glob {
        inline bool copied = false;
        inline bool moved = false;
    }

    bool moveCalled() {
        return glob::moved;
    }

    bool copyCalled() {
        return glob::copied;
    }

    void resetFlags() {
        glob::copied = false;
        glob::moved = false;
    }
}

#endif

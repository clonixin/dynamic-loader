/**
** \file OpenFlags.cpp
**
** \author Phantomas <phantomas@phantomas.xyz>
** \date Created on: 2020-04-28 16:37
** \date Last update: 2020-04-28 16:37
** \copyright GNU Lesser Public Licence v3
*/

#include "./OpenFlags.hpp"

namespace clonixin::dynamicloader::backends::_linux {
    OpenFlags operator|(OpenFlags lhs, OpenFlags rhs) {
        return static_cast<OpenFlags>(static_cast<int>(lhs) | static_cast<int>(rhs));
    }

    OpenFlags &operator|=(OpenFlags& lhs, OpenFlags rhs) {
        lhs = static_cast<OpenFlags>(static_cast<int>(lhs) | static_cast<int>(rhs));
        return lhs;
    }
}

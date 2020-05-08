/**
** \file OpenFlags.hpp
**
** \author Phantomas <phantomas@phantomas.xyz>
** \date Created on: 2020-04-28 16:37
** \date Last update: 2020-04-28 16:39
** \copyright GNU Lesser Public Licence v3
*/

#ifndef OpenFlags_hpp_
#define OpenFlags_hpp_

#include <dlfcn.h>

namespace clonixin::dynamicloader::backends::_linux {
    enum struct OpenFlags : int {
        Default = RTLD_LAZY|RTLD_LOCAL,
        Lazy = RTLD_LAZY,
        Now = RTLD_NOW,
        Global = RTLD_GLOBAL,
        Local = RTLD_LOCAL,
        NoDelete = RTLD_NODELETE,
        NoLoad = RTLD_NOLOAD,
        DeepBind = RTLD_DEEPBIND
    };

    OpenFlags operator|(OpenFlags lhs, OpenFlags rhs);
    OpenFlags &operator|=(OpenFlags& lhs, OpenFlags rhs);
}


#endif

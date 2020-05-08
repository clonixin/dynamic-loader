/**
** \file backends.hpp
** Linux convenience header to include every implementation at once.
**
** \author Phantomas <phantomas@phantomas.xyz>
** \date Created on: 2020-04-27 17:37
** \date Last update: 2020-05-08 15:39
** \copyright GNU Lesser Public Licence v3
*/

#ifndef backends_linux_hpp_
#define backends_linux_hpp_

    #include "./LinuxBackend.hpp"

    #ifdef _GNU_SOURCE
        #include "./LinuxScopedBackend.hpp"
    #endif

namespace clonixin::dynamicloader::backends {
    using DefaultBackend = _linux::LinuxBackend;

    #ifdef _GNU_SOURCE
        using ScopedBackend = _linux::LinuxScopedBackend;
    #endif

    /**
    ** \namespace clonixin::dynamicloader::backends::_linux
    ** \brief Contains linux specific backends.
    */
    namespace _linux {}
}

#endif

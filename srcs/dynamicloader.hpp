/**
** \file dynamicloader.hpp
** Main header file. Include every needed files used to dynamically load
** libraries.
**
** Also contains convenience using, to remove most namespaces.
**
** \author phantomas <phantomas@phantomas.xyz>
** \date Creation: 2020-04-19 02:15
** \date Last update: 2020-04-29 02:23
** \copyright GNU Lesser Public Licence v3
*/

#ifndef dynamicloader_hpp_
#define dynamicloader_hpp_

#include "BasicLoader/BasicLoader.hpp"
#include "exceptions/exceptions.hpp"

#include "backends/backends.hpp"

/**
** \namespace clonixin::dynamicloader
** \brief Main namespace for Clonixin's dynamicloader library.
**
** This namespace contains all classes used in the library, the main wrapper,
** and an alias on the default backend, depending on the target platform.
*/
namespace clonixin::dynamicloader {
    using DefaultLoader = BasicLoader<DefaultBackend>;
}

#endif

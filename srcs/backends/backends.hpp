/**
** \file backends.hpp
** Backend convenience header file.
**
** The point of this file is to include any available backends for the target
** build architecture.
**
** \author Phantomas <phantomas@phantomas.xyz>
** \date Created on: 2020-04-27 17:38
** \date Last update: 2020-04-29 01:03
** \copyright GNU Lesser Public Licence v3
*/

#ifndef dynamicloader_backends_hpp_
#define dynamicloader_backends_hpp_

#ifdef __linux__
#include "./linux/backends.hpp"
#endif

/**
** \namespace clonixin::dynamicloader::backends
** \brief Contains every backends for Clonixin's dynamicloader library.
*/
namespace clonixin::dynamicloader::backends {
}

#endif

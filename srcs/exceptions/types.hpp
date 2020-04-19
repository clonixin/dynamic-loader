/**
** \file exceptions/types.hpp
** Header file containing exceptions::Type enum declaration, used for to
** specialize DLException.
**
** \author phantomas <phantomas@phantomas.xyz>
** \date Creation: 2020-04-19 02:34
** \date Last Update: 2020-04-19 19:21
** \copyright GNU Lesser Public Licence v3
*/

#ifndef EXCEPTIONS_TYPES_HPP__
#define EXCEPTIONS_TYPES_HPP__

namespace clonixin::dynamicloader::exceptions {

    /**
    ** \enum exceptions::Type
    ** \brief This enum represent the different errors that the library could
    ** throw.
    **
    ** The goal of this type is to specialize DLException, so that it can be
    ** easily catched, for a specific type of error, without having to write
    ** any other exceptions types.
    */
    enum struct Type {
        /**
        ** Represent an error during the construction of the backend
        ** or the opening of the dynamic library file.
        */
        Open,

        /**
        ** Represent an error during some symbol lookup. Usually it is thrown
        ** if the symbol was not found.
        */
        LoadSym,

        /**
        ** Represent an error during some symbol lookup. Usually it is thrown
        ** if the symbol was found, but is nullptr, and the user was trying to
        ** get de symbol using move or copy semantic.
        */
        NullSym,

        /**
        ** Represent an error during a Backend destruction. This usually means
        ** that there was some error while closing the handle to the lib.
        */
        Close
    };
}

#endif

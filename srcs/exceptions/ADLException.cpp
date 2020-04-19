/**
** \file exceptions/ADLException.cpp
** Implementation file for ADLException functions.
**
** \author phantomas <phantomas@phantomas.xyz>
** \date Creation: 2020-04-19 19:31
** \date Last Update: 2020-04-19 20:20
** \copyright GNU Lesser Public Licence v3
*/

#include "./ADLException.hpp"

namespace clonixin::dynamicloader::exceptions {
    /**
    ** \brief std::string based constructor.
    **
    ** \param name Name of the symbol or path of the file, whose access
    ** triggered the exception.
    ** \param what Human-ish readable string, describing the error.
    */
    ADLException::ADLException(std::string name, std::string what)
        : std::runtime_error(what), _name(name) {}

    /**
    ** \brief C-style string based constructor.
    **
    ** \param name Name of the symbol or path of the file that triggered
    ** the exception.
    ** \param what Human-ish readable string, describing the error.
    */
    ADLException::ADLException(char const *name, char const * what)
        : std::runtime_error(what), _name(name) {}

    /**
    ** \brief Get the name of the symbol of file that caused the exception.
    **
    ** \return Name of the symbol, or path of the file, whose access triggered
    ** the exception.
    */
    std::string ADLException::getName() const noexcept {
        return _name;
    }
}

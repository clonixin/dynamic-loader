/**
** \file exceptions/DLException.hpp
** Header for the templated class DLException.
**
** \author phantomas <phantomas@phantomas.xyz>
** \date Creation: 2020-04-19 19:39
** \date Last update: 2020-04-28 16:32
** \copyright GNU Lesser Public Licence v3
*/

#ifndef EXCEPTION_EXCEPTION_HPP__
#define EXCEPTION_EXCEPTION_HPP__

#include "./ADLException.hpp"

namespace clonixin::dynamicloader::exceptions {

    /**
    ** \class DLException
    ** \brief Templated class to throw meaningful exceptions easily.
    **
    ** This class is the only one thrown by this library. It's use of template
    ** enable a developer to add new type of error if needed, without changing
    ** the class.
    **
    ** It's goal is to be able to differentiate exception by their types in a
    ** catch clause.
    **
    ** \tparam type The type of error that happened.
    */
    template <Type type>
    class DLException : public ADLException {
        public:
            DLException(std::string name, std::string what);
                //: ADLException(name, what) {}
            DLException(char const *name, char const *what);
                //: ADLException(name, what) {}

            virtual ~DLException() {}

            Type getType() const noexcept final;
    };


    /**
    ** \brief std::string based constructor.
    **
    ** \param name Name of the symbol or path of the file, whose access
    ** triggered the exception.
    ** \param what Human-ish readable string, describing the error.
    **
    ** \tparam type The type of error that happened.
    */
    template <Type type>
    DLException<type>::DLException(std::string name, std::string what)
        : ADLException(name, what) {}

    /**
    ** \brief C-style string based constructor.
    **
    ** \param name Name of the symbol or path of the file that triggered
    ** the exception.
    ** \param what Human-ish readable string, describing the error.
    **
    ** \tparam type The type of error that happened.
    */
    template <Type type>
    DLException<type>::DLException(char const *name, char const *what)
        : ADLException(name, what) {}

    /**
    ** \brief Get the type of the symbol of error that caused the exception.
    **
    ** This getter allow for easy access to the template parameter type, that
    ** describe the kind of error that caused the exception.
    **
    ** \tparam type The type of error that happened.
    **
    ** \return This function effectively return the template parameter type.
    */
    template <Type type>
    Type DLException<type>::getType() const noexcept {
        return type;
    }
}

#endif

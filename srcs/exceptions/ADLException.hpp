/**
** \file exceptions/ADLException.hpp
** Header for the abstract class ADLException.
**
** \author phantomas <phantomas@phantomas.xyz>
** \date Creation: 2020-04-19 19:23
** \date Last Update: 2020-04-19 19:53
** \copyright GNU Lesser Public Licence v3
*/

#ifndef EXCEPTION_AEXCEPTION_HPP__
#define EXCEPTION_AEXCEPTION_HPP__

#include <stdexcept>

#include "./types.hpp"

namespace clonixin::dynamicloader::exceptions {
    /**
    ** \class ADLException
    ** \brief Abstract class for easy catching of exceptions.
    **
    ** This class is an abstract class from which DLException inherits.
    **
    ** It's goal is for all DLException specialization to be easily caught, as
    ** well as to implement some base functions.
    **
    ** \fn std::string ADLException::getType() const noexcept
    ** \brief Get the type of error that caused the exception.
    **
    ** \var std::string ADLException::_name
    ** \brief The name of the symbol or file whose access caused the exception.
    */
    class ADLException : public std::runtime_error {
        public:
            ADLException(std::string name, std::string what);
            ADLException(char const *name, char const * what);

            virtual ~ADLException() {};
            virtual Type getType() const noexcept = 0;
            virtual std::string getName() const noexcept;

        private:
            std::string _name;
    };
}

#endif

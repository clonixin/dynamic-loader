/**
** \file BasicLoader/BasicLoader.hpp
** Header for BasicLoader.
**
** \author phantomas <phantomas@phantomas.xyz>
** \date Creation: 2020-04-17 00:56
** \date Last update: 2020-04-29 02:23
** \copyright GNU Lesser Public Licence v3
*/

#ifndef BasicLoader_hpp_
#define BasicLoader_hpp_

#include <string>
#include <optional>
#if __cplusplus < 201703L
    #warning "C++ version should be c++17 or higher."
#endif
#include <type_traits>

#include "utils/sfinae.hpp"
#include "exceptions/DLException.hpp"

namespace clonixin::dynamicloader {
    namespace cde = clonixin::dynamicloader::exceptions;

    /**
    ** \class BasicLoader
    ** \brief Wrapper class using template to retrieve typed symbols from dynamic libraries.
    **
    ** This class is meant as a frontend for a class enabling the use of
    ** dynamic libraries.It make heavy use of template, to be able to return a
    ** properly typed pointer, or the value it's point to, in a generic and
    ** platform agnostic way,
    **
    ** The Backend class should at least have a constructor accepting the path
    ** of the object to be loaded. It should also be move constructible and move
    ** assignable.
    **
    ** The backend must also provide at least the following functions:
    **   - bool hasError() noexcept, which is called to check whether the last
    **     operation caused an error or not.
    **   - bool %hasSymbol() noexcept, which is called to check whether a given
    **     symbol exists or not.
    **   - std::string getPath() noexcept, a function returning the path of the
    **     opened dynamic library.
    **   - SymAddr getSymbol(std::string const name) noexcept, which is called
    **     when retrieving a symbol, and should return an opaque type, that can
    **     be compared to nullptr, and reinterpreted as any types.
    **   - std::string getPath() const noexcept, called if the path of the resource
    **     is needed.
    **   - std::string getLastError() noexcept, called on error to retrieve
    **     information on what caused it.
    **   - bool reset(...) A function to reset the backend, which should accept
    **     the same parameters as one of it's constructors, and return a bool
    **     denoting whether the operation was successful or not. On error, the
    **     backend should not be modified.
    **
    ** It's assumed that the backend will follow RAII pattern, and manage the
    ** lifetime of it's handle. It's highly encouraged to handle the move
    ** operations without opening a new handle, as the wrapper will discard the
    ** old value, which in turn will call the destructor.
    **
    ** \tparam Backend Type of the backend class.
    */
    template <class Backend>
    class BasicLoader {
        public:
            template <typename... Args>
            BasicLoader(std::string const &, Args &&... args);
            explicit BasicLoader(Backend &&bck) noexcept;
            BasicLoader(BasicLoader const &oth) = delete;
            BasicLoader(BasicLoader && oth) noexcept;
            virtual ~BasicLoader();

            BasicLoader &operator=(BasicLoader const &) = delete;
            BasicLoader &operator=(Backend &&rhs) noexcept;
            BasicLoader &operator=(BasicLoader &&rhs) noexcept;

            template <typename... Args>
            bool reset(std::string const &path, Args &&... args) noexcept;
            bool reset(Backend &&bck) noexcept;

            [[nodiscard]]
            bool hasSymbol(std::string const &name) const noexcept;

            /* ifptr_t<T> functions */
            template <typename T>
            [[nodiscard]]
            ifptr_t<T> getSymbol(std::string const &name) const;

            template <typename T>
            [[nodiscard]]
            std::optional<ifptr_t<T>> tryGetSymbol(std::string const &name, cde::Type &err_type, std::string &err_out) const noexcept;

            template <typename T>
            [[nodiscard]]
            std::optional<ifptr_t<T>> tryGetSymbol(std::string const &name) const noexcept;
            /* !ifptr_t<T> functions */

            /* iflref_t<T> functions */
            template <typename T>
            [[nodiscard]]
            iflref_t<T> getSymbol(std::string const &name) const;

            template <typename T>
            [[nodiscard]]
            std::optional<refw<iflref_t<T>>> tryGetSymbol(std::string const &name, cde::Type &err_type, std::string &err_out) const noexcept;

            template <typename T>
            [[nodiscard]]
            std::optional<refw<iflref_t<T>>> tryGetSymbol(std::string const &name) const noexcept;
            /* !iflref_t<T> functions */

            /* ifmove_t<T> functions */
            template <typename T>
            [[nodiscard]]
            ifmove_t<T> getSymbol(std::string const &name) const;

            template <typename T>
            [[nodiscard]]
            std::optional<ifmove_t<T>> tryGetSymbol(std::string const &name, cde::Type &err_type, std::string &err_out) const noexcept;

            template <typename T>
            [[nodiscard]]
            std::optional<ifmove_t<T>> tryGetSymbol(std::string const &name) const noexcept;
            /* !ifmove_t<T> functions */

            /* ifcopy_t<T> functions */
            template <typename T>
            [[nodiscard]]
            ifcopy_t<T> getSymbol(std::string const &name) const;

            template <typename T>
            [[nodiscard]]
            std::optional<ifcopy_t<T>> tryGetSymbol(std::string const &name, cde::Type &err_type, std::string &err_out) const noexcept;

            template <typename T>
            [[nodiscard]]
            std::optional<ifcopy_t<T>> tryGetSymbol(std::string const &name) const noexcept;
            /* !ifcopy_t<T> functions */

            /* iferror_t<T> functions */
            template <typename T>
            [[noreturn]]
            iferror_t<T> getSymbol(std::string const &name) const;

            template <typename T>
            [[noreturn]]
            std::optional<iferror_t<T>> tryGetSymbol(std::string const &name, cde::Type &err_type, std::string &err_out) const noexcept;

            template <typename T>
            [[noreturn]]
            std::optional<iferror_t<T>> tryGetSymbol(std::string const &name) const noexcept;
            /* !iferror_t<T> functions */

            [[nodiscard]]
            Backend &accessBackend();
        private:
            mutable Backend     _backend;
    };

    /**
    ** \brief Standard constructor
    **
    ** This constructor initialise the underlying backend by calling it's
    ** constructor.
    **
    ** \param path Path of the file to load
    ** \param args Variadic list of optionnal arguments, that will be passed as-is to the constructor.
    **
    ** \tparam Backend Type of the backend object.
    ** \tparam Args Variadic list of types to pass to the backend.
    */
    template <class Backend>
    template <typename... Args>
    BasicLoader<Backend>::BasicLoader(std::string const &path, Args &&... args)
    : _backend(path, std::forward<Args>(args)...)
    {
        if (_backend.hasError())
            throw cde::DLException<cde::Type::Open>(path, _backend.getLastError());
    }

    /**
    ** \brief rvalue initialization constructor
    **
    ** This constructor move construct the BasicLoader backend.
    **
    ** \param bck Object to be std::move'd
    **
    ** \tparam Backend Type of the backend object.
    **
    ** \warning The status of the backend is not checked in this function.
    ** It is assumed that it's in a valid state, as it has been created by the
    ** developer calling explicitly this function.
    */
    template <class Backend>
    BasicLoader<Backend>::BasicLoader(Backend &&bck) noexcept: _backend(std::move(bck)) {}

    /**
    ** \brief Move constructor
    **
    ** This constructor initialize the object backend by std::move-ing the
    ** other object backend.
    **
    ** \param oth BasicLoader to be moved.
    **
    ** \tparam Backend Type of the backend object.
    */
    template <class Backend>
    BasicLoader<Backend>::BasicLoader(BasicLoader && oth) noexcept : _backend(oth._backend) {}

    /**
    ** \brief BasicLoader Destructor
    **
    ** \tparam Backend Type of the backend object.
    **
    ** \warning As it is assumed that the backend follow RAII principle, this destructor
    ** does nothing. As such, is it the reponsibility of the backend to free or close any
    ** resources.
    */
    template <class Backend>
    BasicLoader<Backend>::~BasicLoader() {}

    /**
    ** \fn  BasicLoader<Backend, Args ...> & BasicLoader::operator=(BasicLoader &&rhs) noexcept
    ** \brief Move assignment operator
    **
    ** This operator call the move assignment operator on it's backend by
    ** std::move-ing the backend of the other object.
    **
    ** \param rhs BasicLoader to be moved.
    **
    ** \tparam Backend Type of the backend object.
    **
    ** \return Returns a reference on the object in the left hand side of the operator
    */
    template <class Backend>
    BasicLoader<Backend>& BasicLoader<Backend>::operator=(BasicLoader<Backend> &&rhs) noexcept {
        _backend = std::move(rhs._backend);
        return *this;
    }

    /**
    ** \brief rvalue backend assignation.
    **
    ** This operator move assign the BasicLoader backend.
    **
    ** \param rhs Backend object to be std::move'd
    **
    ** \tparam Backend Type of the backend object.
    **
    ** \return Returns a reference on the object in the left hand side of the operator
    **
    ** \warning The status of the backend is not checked in this function.
    ** It is assumed that it's in a valid state, as it has been created by the
    ** developer calling explicitly this function.
    */
    template <class Backend>
    BasicLoader<Backend>& BasicLoader<Backend>::operator=(Backend &&rhs) noexcept {
        _backend = std::move(rhs);
        return *this;
    }

    /**
    ** \brief Reset the underlying backend.
    **
    ** This function calls the backend reset function, with a new path and args.
    **
    ** \param path Path of the file to load
    ** \param args Variadic list of optional arguments, that will be passed as-is to the constructor.
    **
    ** \tparam Backend Type of the backend object.
    ** \tparam Args Variadic list of types to pass to the backend.
    **
    ** \return If the reset is successful, this function return true. Otherwise,
    ** false is returned. The backend should not be modified if false is returned.
    */
    template <class Backend>
    template <typename... Args>
    bool BasicLoader<Backend>::reset(std::string const &path, Args &&... args) noexcept {
        return _backend.reset(path, std::forward<Args>(args)...);
    }

    /**
    ** \brief Reset the BasicLoader
    **
    ** A call to this function is equivalent to a call to the move assignment
    ** BasicLoader::operator=(Backend &&rhs), except that it returns true on success.
    **
    ** \param bck Backend object to be std::move'd
    **
    ** \tparam Backend Type of the backend object.
    **
    ** \return If the reset is successful, this function return true. Otherwise,
    ** false is returned. The backend should not be modified, if false is returned.
    **
    ** \warning The status of the backend is not checked in this function.
    ** It is assumed that it's in a valid state, as it has been created by the
    ** developer calling explicitly this function.
    */
    template <class Backend>
    bool BasicLoader<Backend>::reset(Backend && bck) noexcept {
        _backend = std::move(bck);
        return true;
    }

    /**
    ** \brief Look for a given symbol
    **
    ** This is used to check if a given symbol exists in the loaded library.
    ** Use this function if you use a symbol name as a magic value, but don't
    ** need the actual value.
    **
    ** \param name The name of the symbol needed.
    **
    ** \tparam Backend Type of the backend object.
    **
    ** \return This function return true if the symbol returns.
    */
    template <class Backend>
    bool BasicLoader<Backend>::hasSymbol(std::string const &name) const noexcept {
        return _backend.hasSymbol(name);
    }

    /**
    ** \name Pointer returning symbols getters
    **
    ** These function are activated or de-activated via SFINAE.
    ** When ifptr_t<T> has a type, meaning T is a pointer, the functions in
    ** this group can be compiled.
    **
    ** The BasicLoader::getSymbol and BasicLoader::tryGetSymbol will then
    ** return the address of the symbol properly typed.
    */
    /**@{*/

    /**
    ** \brief Get the address of the symbol name.
    **
    ** This function calls Backend::getSymbol then return a reinterpreted
    ** pointer of type T.
    **
    ** \note Internally, the return value of Backend::getSymbol is stored in an
    ** opaque type, defined by Backend, whose value must be comparable to
    ** nullptr. It must also be possible to call reinterpret_cast on it.
    **
    ** \param name The name of the symbol to retrieve.
    **
    ** \tparam Backend Type of the backend object.
    ** \tparam T A type for which std::is_pointer_v<T> is true.
    **
    ** \return This function will return the address of the symbol, if found.
    ** It'll throw an exception if an error occurred in the backend. Most of
    ** the time, the returned pointer will not be nullptr, although in some
    ** specific case, it could.
    **
    ** \throw DLException<LoadSym> if the symbol could not be found. That is,
    ** if the returned symbol is equal to nullptr, and Backend#hasError()
    ** returns true.
    */
    template <class Backend>
    template <typename T>
    ifptr_t<T> BasicLoader<Backend>::getSymbol(std::string const &name) const {
        void * sym = _backend.getSymbol(name);

        if (sym == nullptr && _backend.hasError())
            throw cde::DLException<cde::Type::LoadSym>(name, _backend.getLastError());

        return reinterpret_cast<T>(sym);
    }

    /**
    ** \brief Optionally get the address of a given symbol.
    **
    ** This function calls Backend::getSymbol then return a reinterpreted
    ** pointer of type T.
    **
    ** It's call is equivalent to BasicLoader::getSymbol, except that it will
    ** never throw, returning std::nullopt instead
    **
    ** \note Internally, the return value of Backend::getSymbol is stored in an
    ** opaque type, defined by Backend, whose value must be comparable to
    ** nullptr. It must also be possible to call reinterpret_cast on it.
    **
    ** \param name The name of the symbol to retrieve.
    ** \param err_type A reference on a exceptions::Type which will be changed
    ** to reflect the kind of error, if needed.
    ** \param err_out A reference on a std::string on which the assignment
    ** operator will be called with a string describing the error.
    **
    ** \tparam Backend Type of the backend object.
    ** \tparam T A type for which std::is_pointer_v<T> is true.
    **
    ** \return This function will return the address of the symbol, if found,
    ** or std::nullopt.
    */
    template <class Backend>
    template <typename T>
    std::optional<ifptr_t<T>> BasicLoader<Backend>::tryGetSymbol(std::string const &name, cde::Type &err_type, std::string &err_out) const noexcept {
        typename Backend::SymAddr sym = _backend.getSymbol(name);

        if (sym == nullptr && _backend.hasError()) {
            err_type = cde::Type::LoadSym;
            err_out = _backend.getLastError();
            return std::nullopt;
        }

        return reinterpret_cast<T>(sym);
    }

    /**
    ** \brief Optionally get the address of a given symbol.
    **
    ** This function calls Backend::getSymbol then return a reinterpreted
    ** pointer of type T.
    **
    ** It's call is equivalent to BasicLoader::getSymbol, except that it will
    ** never throw, returning std::nullopt instead
    **
    ** \note Internally, the return value of Backend::getSymbol is stored in an
    ** opaque type, defined by Backend, whose value must be comparable to
    ** nullptr. It must also be possible to call reinterpret_cast on it.
    **
    ** \param name The name of the symbol to retrieve.
    **
    ** \tparam Backend Type of the backend object.
    ** \tparam T A type for which std::is_pointer_v<T> is true.
    **
    ** \return This function will return the address of the symbol, if found,
    ** or std::nullopt.
    */
    template <class Backend>
    template <typename T>
    std::optional<ifptr_t<T>> BasicLoader<Backend>::tryGetSymbol(std::string const &name) const noexcept {
        std::string discard;
        cde::Type tdiscard;
        return tryGetSymbol<T>(name, tdiscard, discard);
    }
    /**@}*/

    /**
    ** \name Lvalue reference symbols getters.
    **
    ** These function are activated or de-activated via SFINAE.
    ** When iflref_t<T> has a type, meaning T is a lvalue reference.
    **
    ** The BasicLoader::getSymbol and BasicLoader::tryGetSymbol will then
    ** return the properly typed symbol, by dereferencing it.
    */
    /**@{*/

    /**
    ** \brief Get a given symbol by lvalue reference.
    **
    ** This function calls Backend::getSymbol, reinterpret_cast it's return
    ** value, then return by dereference it.
    **
    ** \note Internally, the return value of Backend::getSymbol is stored in an
    ** opaque type, defined by Backend, whose value must be comparable to
    ** nullptr. It must also be possible to call reinterpret_cast on it.
    **
    ** \param name The name of the symbol to retrieve.
    **
    ** \tparam Backend Type of the backend object.
    ** \tparam T A lvalue reference type.
    **
    ** \return This function will return the symbol, if found.
    ** It'll throw an exception if an error occurred in the backend, or if the
    ** symbol is equal to nullptr, as it wouldn't be possible to dereference it
    ** if that was the case.
    **
    ** \throw DLException<LoadSym> Thrown if the symbol could not be found.
    ** That is if the returned symbol is equal to nullptr, and
    ** Backend#hasError() returns true.
    **
    ** \throw DLException<NullSym> if the backend has no error, and the value
    ** returned by Backend::getSymbol is equal to nullptr.
    */
    template <class Backend>
    template <typename T>
    iflref_t<T> BasicLoader<Backend>::getSymbol(std::string const &name) const {
        typename Backend::SymAddr sym = _backend.getSymbol(name);

        if (sym == nullptr && _backend.hasError())
            throw cde::DLException<cde::Type::LoadSym>(name, _backend.getLastError());
        else if (sym == nullptr)
            throw cde::DLException<cde::Type::NullSym>(name, std::string(_backend.getPath() + ": Symbol " + name + " is NULL and cannot be casted."));

        return *reinterpret_cast<std::remove_reference_t<T> *>(sym);
    }

    /**
    ** \brief Optionally get a given symbol by lvalue reference.
    **
    ** This function calls Backend::getSymbol, reinterpret_cast it's return
    ** value, then return by dereferencing it. The return value is then wrapped
    ** in a std::reference_wrapper, wrapped in a std::optional.
    **
    ** \note Internally, the return value of Backend::getSymbol is stored in an
    ** opaque type, defined by Backend, whose value must be comparable to
    ** nullptr. It must also be possible to call reinterpret_cast on it.
    **
    ** \param name The name of the symbol to retrieve.
    ** \param err_type A reference on a exceptions::Type which will be changed
    ** to reflect the kind of error, if needed.
    ** \param err_out A reference on a std::string on which the assignment
    ** operator will be called with a string describing the error.
    **
    ** \tparam Backend Type of the backend object.
    ** \tparam T A lvalue reference type
    **
    ** \return This function will return the symbol, if found. It'll return
    ** nullopt if an error occurred in the backend, or if the symbol is equal to
    ** nullptr, as it wouldn't be possible to dereference it if that was the
    ** case.
    */
    template <class Backend>
    template <typename T>
    std::optional<refw<iflref_t<T>>> BasicLoader<Backend>::tryGetSymbol(std::string const &name, cde::Type &err_type, std::string &err_out) const noexcept {
        typename Backend::SymAddr sym = _backend.getSymbol(name);

        if (sym == nullptr) {
            if (_backend.hasError()) {
                err_type = cde::Type::LoadSym;
                err_out = _backend.getLastError();
            }
            else {
                err_type = cde::Type::NullSym;
                err_out = _backend.getPath() + ": Symbol " + name + " is NULL and cannot be cast-ed.";
            }

            return std::nullopt;
        }

        return *reinterpret_cast<std::remove_reference_t<T> *>(sym);
    }

    /**
    ** \brief Optionally get a given symbol by lvalue_reference
    **
    ** This function calls Backend::getSymbol, reinterpret_cast it's return
    ** value, then return by dereferencing it. The return value is then wrapped
    ** in a std::reference_wrapper, wrapped in a std::optional.
    **
    ** \note Internally, the return value of Backend::getSymbol is stored in an
    ** opaque type, defined by Backend, whose value must be comparable to
    ** nullptr. It must also be possible to call reinterpret_cast on it.
    **
    ** \param name The name of the symbol to retrieve.
    **
    ** \tparam Backend Type of the backend object.
    ** \tparam T A lvalue reference type
    **
    ** \return This function will return the symbol, if found. It'll return
    ** nullopt if an error occurred in the backend, or if the symbol is equal to
    ** nullptr, as it wouldn't be possible to dereference it if that was the
    ** case.
    */
    template <class Backend>
    template <typename T>
    std::optional<refw<iflref_t<T>>> BasicLoader<Backend>::tryGetSymbol(std::string const &name) const noexcept {
        std::string discard;
        cde::Type tdiscard;
        return tryGetSymbol<T>(name, tdiscard, discard);
    }
    /**@}*/

    /**
    ** \name Move semantic symbols getters.
    **
    ** These function are activated or de-activated via SFINAE.
    ** When ifmove_t<T> has a type, meaning T is not a pointer, and can be move
    ** constructed or assigned, the functions in this group can be compiled.
    **
    ** The BasicLoader::getSymbol and BasicLoader::tryGetSymbol will then
    ** return the properly typed symbol, by dereferencing then moving it.
    */
    /**@{*/

    /**
    ** \brief Get a given symbol using move semantics.
    **
    ** This function calls Backend::getSymbol, reinterpret_cast it's return
    ** value, then return by dereference it, and calling std::move.
    **
    ** \note Internally, the return value of Backend::getSymbol is stored in an
    ** opaque type, defined by Backend, whose value must be comparable to
    ** nullptr. It must also be possible to call reinterpret_cast on it.
    **
    ** \param name The name of the symbol to retrieve.
    **
    ** \tparam Backend Type of the backend object.
    ** \tparam T A non-pointer move constructible or move assignable type.
    **
    ** \return This function will return the symbol, if found.
    ** It'll throw an exception if an error occurred in the backend, or if the
    ** symbol is equal to nullptr, as it wouldn't be possible to dereference it
    ** if that was the case.
    **
    ** \throw DLException<LoadSym> Thrown if the symbol could not be found.
    ** That is if the returned symbol is equal to nullptr, and
    ** Backend#hasError() returns true.
    **
    ** \throw DLException<NullSym> if the backend has no error, and the value
    ** returned by Backend::getSymbol is equal to nullptr.
    */
    template <class Backend>
    template <typename T>
    ifmove_t<T> BasicLoader<Backend>::getSymbol(std::string const &name) const {
        typename Backend::SymAddr sym = _backend.getSymbol(name);

        if (sym == nullptr && _backend.hasError())
            throw cde::DLException<cde::Type::LoadSym>(name, _backend.getLastError());
        else if (sym == nullptr)
            throw cde::DLException<cde::Type::NullSym>(name, std::string(_backend.getPath() + ": Symbol " + name + " is NULL and cannot be casted."));

        return std::move(*reinterpret_cast<T *>(sym));
    }

    /**
    ** \brief Optionally get a given symbol using move semantic.
    **
    ** This function calls Backend::getSymbol, reinterpret_cast it's return
    ** value, then return by dereferencing it, and calling std::move. The
    ** return value is then wrapped in a std::optional.
    **
    ** \note Internally, the return value of Backend::getSymbol is stored in an
    ** opaque type, defined by Backend, whose value must be comparable to
    ** nullptr. It must also be possible to call reinterpret_cast on it.
    **
    ** \param name The name of the symbol to retrieve.
    ** \param err_type A reference on a exceptions::Type which will be changed
    ** to reflect the kind of error, if needed.
    ** \param err_out A reference on a std::string on which the assignment
    ** operator will be called with a string describing the error.
    **
    ** \tparam Backend Type of the backend object.
    ** \tparam T A non-pointer move constructible or move assignable type.
    **
    ** \return This function will return the symbol, if found. It'll return
    ** nullopt if an error occurred in the backend, or if the symbol is equal to
    ** nullptr, as it wouldn't be possible to dereference it if that was the
    ** case.
    */
    template <class Backend>
    template <typename T>
    std::optional<ifmove_t<T>> BasicLoader<Backend>::tryGetSymbol(std::string const &name, cde::Type &err_type, std::string &err_out) const noexcept {
        typename Backend::SymAddr sym = _backend.getSymbol(name);

        if (sym == nullptr) {
            if (_backend.hasError()) {
                err_type = cde::Type::LoadSym;
                err_out = _backend.getLastError();
            }
            else {
                err_type = cde::Type::NullSym;
                err_out = _backend.getPath() + ": Symbol " + name + " is NULL and cannot be cast-ed.";
            }

            return std::nullopt;
        }

        return std::move(*reinterpret_cast<T *>(sym));
    }

    /**
    ** \brief Optionally get a given symbol using move semantic.
    **
    ** This function calls Backend::getSymbol, reinterpret_cast it's return
    ** value, then return by dereferencing it, and calling std::move. The
    ** return value is then wrapped in a std::optional.
    **
    ** \note Internally, the return value of Backend::getSymbol is stored in an
    ** opaque type, defined by Backend, whose value must be comparable to
    ** nullptr. It must also be possible to call reinterpret_cast on it.
    **
    ** \param name The name of the symbol to retrieve.
    **
    ** \tparam Backend Type of the backend object.
    ** \tparam T A non-pointer move constructible or move assignable type.
    **
    ** \return This function will return the symbol, if found. It'll return
    ** nullopt if an error occurred in the backend, or if the symbol is equal to
    ** nullptr, as it wouldn't be possible to dereference it if that was the
    ** case.
    */
    template <class Backend>
    template <typename T>
    std::optional<ifmove_t<T>> BasicLoader<Backend>::tryGetSymbol(std::string const &name) const noexcept {
        std::string discard;
        cde::Type tdiscard;
        return tryGetSymbol<T>(name, tdiscard, discard);
    }
    /**@}*/


    /**
    ** \name Copy semantic symbols getters.
    **
    ** These function are activated or de-activated via SFINAE.
    ** When ifcopy_t<T> has a type, meaning T is nor a pointer, and T can be
    ** copy constructed or assigned, the functions in this group can be
    ** compiled.
    **
    ** The BasicLoader::getSymbol and BasicLoader::tryGetSymbol will then
    ** return the properly typed symbol, by dereferencing it.
    */
    /**@{*/

    /**
    ** \brief Get a given symbol using copy semantics.
    **
    ** This function calls Backend::getSymbol, reinterpret_cast it's return
    ** value, then return by dereference it.
    **
    ** \note Internally, the return value of Backend::getSymbol is stored in an
    ** opaque type, defined by Backend, whose value must be comparable to
    ** nullptr. It must also be possible to call reinterpret_cast on it.
    **
    ** \param name The name of the symbol to retrieve.
    **
    ** \tparam Backend Type of the backend object.
    ** \tparam T A non-pointer copy constructible or copy assignable type.
    **
    ** \return This function will return the symbol, if found.
    ** It'll throw an exception if an error occurred in the backend, or if the
    ** symbol is equal to nullptr, as it wouldn't be possible to dereference it
    ** if that was the case.
    **
    ** \throw DLException<LoadSym> Thrown if the symbol could not be found.
    ** That is if the returned symbol is equal to nullptr, and
    ** Backend#hasError() returns true.
    **
    ** \throw DLException<NullSym> if the backend has no error, and the value
    ** returned by Backend::getSymbol is equal to nullptr.
    */
    template <class Backend>
    template <typename T>
    ifcopy_t<T> BasicLoader<Backend>::getSymbol(std::string const &name) const {
        typename Backend::SymAddr sym = _backend.getSymbol(name);

        if (sym == nullptr && _backend.hasError())
            throw cde::DLException<cde::Type::LoadSym>(name, _backend.getLastError());
        else if (sym == nullptr)
            throw cde::DLException<cde::Type::NullSym>(name, std::string(_backend.getPath() + ": Symbol " + name + " is NULL and cannot be casted."));

        return *reinterpret_cast<T *>(sym);
    }

    /**
    ** \brief Optionally get a given symbol using copy semantic.
    **
    ** This function calls Backend::getSymbol, reinterpret_cast it's return
    ** value, then return by dereferencing it. The return value is then wrapped
    ** in a std::optional.
    **
    ** \note Internally, the return value of Backend::getSymbol is stored in an
    ** opaque type, defined by Backend, whose value must be comparable to
    ** nullptr. It must also be possible to call reinterpret_cast on it.
    **
    ** \param name The name of the symbol to retrieve.
    ** \param err_type A reference on a exceptions::Type which will be changed
    ** to reflect the kind of error, if needed.
    ** \param err_out A reference on a std::string on which the assignment
    ** operator will be called with a string describing the error.
    **
    ** \tparam Backend Type of the backend object.
    ** \tparam T A non-pointer copy constructible or copy assignable type.
    **
    ** \return This function will return the symbol, if found. It'll return
    ** nullopt if an error occurred in the backend, or if the symbol is equal to
    ** nullptr, as it wouldn't be possible to dereference it if that was the
    ** case.
    */
    template <class Backend>
    template <typename T>
    std::optional<ifcopy_t<T>> BasicLoader<Backend>::tryGetSymbol(std::string const &name, cde::Type &err_type, std::string &err_out) const noexcept {
        typename Backend::SymAddr sym = _backend.getSymbol(name);

        if (sym == nullptr) {
            if (_backend.hasError()) {
                err_type = cde::Type::LoadSym;
                err_out = _backend.getLastError();
            }
            else {
                err_type = cde::Type::NullSym;
                err_out = _backend.getPath() + ": Symbol " + name + " is NULL and cannot be cast-ed.";
            }
            return std::nullopt;
        }

        return *reinterpret_cast<T *>(sym);
    }

    /**
    ** \brief Optionally get a given symbol using copy semantic.
    **
    ** This function calls Backend::getSymbol, reinterpret_cast it's return
    ** value, then return by dereferencing it. The return value is then wrapped
    ** in a std::optional.
    **
    ** \note Internally, the return value of Backend::getSymbol is stored in an
    ** opaque type, defined by Backend, whose value must be comparable to
    ** nullptr. It must also be possible to call reinterpret_cast on it.
    **
    ** \param name The name of the symbol to retrieve.
    **
    ** \tparam Backend Type of the backend object.
    ** \tparam T A non-pointer copy constructible or copy assignable type.
    **
    ** \return This function will return the symbol, if found. It'll return
    ** nullopt if an error occurred in the backend, or if the symbol is equal to
    ** nullptr, as it wouldn't be possible to dereference it if that was the
    ** case.
    */
    template <class Backend>
    template <typename T>
    std::optional<ifcopy_t<T>> BasicLoader<Backend>::tryGetSymbol(std::string const &name) const noexcept {
        std::string discard;
        cde::Type tdiscard;
        return tryGetSymbol<T>(name, tdiscard, discard);
    }
    /**@}*/


    /**
    ** \name Error handling Symbols Getter
    **
    ** These function are activated or de-activated via SFINAE.
    ** When iferror_t<T> has a type, meaning T is a non-pointer, non-movable
    ** and non copyable type, and the functions in this group can be compiled.
    **
    ** The BasicLoader::getSymbol and BasicLoader::tryGetSymbol will not be
    ** compiled however, but will trigger a failure on a static assertion which
    ** will make the compiler fail with a meaningful error.
    **
    ** \note In the unlikely case these function would be compiled, a runtime
    ** error will be thrown to prevent undefined behavior. This error could be
    ** catch if BasicLoader::getSymbol was called, but as the std::optional
    ** returning functions are marked as noexcept, these one will make the
    ** program to terminate.
    */
    /**@{*/

    /**
    ** \brief Fallthrough implementation containing static_assert
    **
    ** This function is only there so there is a proper message returned by the
    ** compiler, using static_cast.
    **
    ** \param name The name of the symbol to retrieve.
    **
    ** \tparam Backend Type of the backend object.
    ** \tparam T A non-pointer, non-copyable, non-movable type.
    **
    ** \return This function will never return.
    **
    ** \throw std::runtime_error if the function were to be somehow executed,
    ** even tho it should not be possible.
    */
    template <class Backend>
    template <typename T>
    iferror_t<T> BasicLoader<Backend>::getSymbol(std::string const &name) const {
        static_assert(!iserror_v<T>, "Type is neither copyable nor movable. You should use a pointer instead.");
        throw std::runtime_error("This function should not have been compiled.");
    }

    /**
    ** \brief Fallthrough implementation containing static_assert
    **
    ** This function is only there so there is a proper message returned by the
    ** compiler, using static_cast.
    **
    ** \param name The name of the symbol to retrieve.
    ** \param err_type A reference on a exceptions::Type which will be changed
    ** to reflect the kind of error, if needed.
    ** \param err_out A string that'll contains the error if any.
    **
    ** \tparam Backend Type of the backend object.
    ** \tparam T A non-pointer, non-copyable, non-movable type.
    **
    ** \return This function will never return.
    **
    ** \throw std::runtime_error if the function were to be somehow executed,
    ** even tho it should not be possible.
    */
    template <class Backend>
    template <typename T>
    std::optional<iferror_t<T>> BasicLoader<Backend>::tryGetSymbol(std::string const &name, cde::Type &err_type, std::string &err_out) const noexcept {
        static_assert(!iserror_v<T>, "Type is neither copyable nor movable. You should use a pointer instead.");
        throw std::runtime_error("This function should not have been compiled.");
    }

    /**
    ** \brief Fallthrough implementation containing static_assert
    **
    ** This function is only there so there is a proper message returned by the
    ** compiler, using static_cast.
    **
    ** \param name The name of the symbol to retrieve.
    **
    ** \tparam Backend Type of the backend object.
    ** \tparam T A non-pointer, non-copyable, non-movable type.
    **
    ** \return This function will never return.
    **
    ** \throw std::runtime_error if the function were to be somehow executed,
    ** even tho it should not be possible.
    */
    template <class Backend>
    template <typename T>
    std::optional<iferror_t<T>> BasicLoader<Backend>::tryGetSymbol(std::string const &name) const noexcept {
        static_assert(!iserror_v<T>, "Type is neither copyable nor movable. You should use a pointer instead.");
        throw std::runtime_error("This function should not have been compiled.");
    }
    /** @} */

    /**
    ** \brief Gives direct access to the backend object.
    **
    ** This function is there in case the underlying backend has method not
    ** available througn this wrapper, such as OS specific features.
    **
    ** It's not considered safe to use, and does not enable the use of
    ** methods in this class, but could still prove useful in some scenarii.
    **
    ** \return Return a reference on the backend of the current object.
    **
    ** \tparam Backend Type of the backend object.
    */
    template <class Backend>
    Backend & BasicLoader<Backend>::accessBackend() {
        return _backend;
    }
} // namespace clonixin::DLoader

#endif

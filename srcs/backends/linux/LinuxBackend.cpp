/**
** \file LinuxBackend.cpp
**
** \author Phantomas <phantomas@phantomas.xyz>
** \date Created on: 2020-04-28 16:36
** \date Last update: 2020-04-28 16:36
** \copyright GNU Lesser Public Licence v3
*/

#include "./LinuxBackend.hpp"

namespace clonixin::dynamicloader::backends::_linux {
    LinuxBackend::LinuxBackend() noexcept {}

    LinuxBackend::LinuxBackend(std::string const &path, void *hndl) noexcept
    : _path(path), _hndl(hndl), _has_error(false), _err_str()
    {}

    LinuxBackend::LinuxBackend(std::string const &path, OpenFlags f) noexcept
    : _path(path) {
        resetError();
        _hndl = dlopen(path.c_str(), static_cast<int>(f));

        symbolError();
    }

    LinuxBackend::LinuxBackend(LinuxBackend &&oth) noexcept :
    _path(std::move(oth._path)), _hndl(oth._hndl),
    _has_error(oth._has_error), _err_str(std::move(oth._err_str)) {
        oth._hndl = nullptr;
        oth._has_error = false;
        oth._err_str.clear();
        oth._path.clear();
    }

    LinuxBackend::~LinuxBackend() {
       if ( _hndl != nullptr and _hndl != GlobHndl && _hndl != NextHndl)
           dlclose(_hndl);
    }

    LinuxBackend &LinuxBackend::operator=(LinuxBackend &&rhs) noexcept {
        if (!(this == std::addressof(rhs))) {
            _path = std::move(rhs._path);

            _hndl = rhs._hndl;
            rhs._hndl = nullptr;

            _has_error = rhs._has_error;
            rhs._has_error = false;

            _err_str = std::move(rhs._err_str);
            rhs._err_str.clear();
        }

        return *this;
    }

    bool LinuxBackend::reset(std::string const &path, OpenFlags f) noexcept {
        resetError();

        void *new_hndl = dlopen(path.c_str(), static_cast<int>(f));

        symbolError();
        if (!_has_error) {
            dlclose(_hndl);
            _hndl = new_hndl;
        }

        return !_has_error;
    }

    std::string LinuxBackend::getPath() const noexcept {
        return _path;
    }

    bool LinuxBackend::hasSymbol(std::string const &name) noexcept {
        (void)getSymbol(name);

        return !_has_error;
    }

    LinuxBackend::SymAddr LinuxBackend::getSymbol(std::string const &name) noexcept {
        resetError();
        void *sym = dlsym(_hndl, name.c_str());

        symbolError();
        return sym != NULL ? sym : nullptr;
    }

    bool LinuxBackend::hasError() const noexcept {
        return _has_error;
    }

    std::string LinuxBackend::getLastError() const noexcept {
        return _err_str;
    }

    void LinuxBackend::resetError() {
        _has_error = false;
        _err_str.clear();

        (void)dlerror();
    }

    void LinuxBackend::symbolError() {
        char *str = dlerror();
        _err_str = (str == NULL ? std::string() : str);
        _has_error = !_err_str.empty();
    }

    LinuxBackend LinuxBackend::InternalSymbolBackend(OpenFlags f) {
        (void)dlerror();
        void *hndl = dlopen(NULL, static_cast<int>(f));
        char *str = dlerror();

        LinuxBackend bck(InternalPath, hndl);
        bck._has_error = str == NULL;
        bck._err_str = str == NULL ? std::string() : str;

        return bck;
    }

    LinuxBackend LinuxBackend::GlobSymbolBackend() {
        return LinuxBackend(GlobPath, GlobHndl);
    }

    LinuxBackend LinuxBackend::NextSymbolBackend() {
        return LinuxBackend(NextPath, NextHndl);
    }
}

/**
** \file LinuxScopedBackend.cpp
**
** \author Phantomas <phantomas@phantomas.xyz>
** \date Created on: 2020-04-28 16:37
** \date Last update: 2020-04-28 16:37
** \copyright GNU Lesser Public Licence v3
*/

#include "./LinuxScopedBackend.hpp"

namespace clonixin::dynamicloader::backends::_linux {
    LinuxScopedBackend::LinuxScopedBackend(std::string const &path, LinuxScopedBackend::Scope s, OpenFlags f)
    : _scope(s) {
        LinuxBackend::resetError();

        _path = path;
        _hndl = dlmopen(_scope.get(), path.c_str(), static_cast<int>(f));

        LinuxBackend::symbolError();

        if (!_has_error && NewScope == _scope) {
            Lmid_t id;

            if (0 != dlinfo(_hndl, RTLD_DI_LMID, &id))
                symbolError();
            else
                _scope = id;
        }
    }

    LinuxScopedBackend::LinuxScopedBackend(LinuxScopedBackend &&oth) noexcept
        : LinuxBackend(std::move(oth)), _scope(std::move(oth._scope)) {}

    LinuxScopedBackend::~LinuxScopedBackend() {}

    LinuxScopedBackend & LinuxScopedBackend::operator =(LinuxScopedBackend &&rhs) noexcept {
        if (this != std::addressof(rhs)) {
            _scope = std::move(rhs._scope);
            reinterpret_cast<LinuxBackend &>(*this) = std::move(reinterpret_cast<LinuxBackend &&>(rhs));
        }

        return *this;
    }

    bool LinuxScopedBackend::reset(std::string const &path, Scope s, OpenFlags f) noexcept {
        LinuxBackend::resetError();

        void *new_hndl = dlmopen(s.get(), path.c_str(), static_cast<int>(f));
        LinuxBackend::symbolError();

        if (!_has_error) {
            if  (NewScope == s) {
                Lmid_t id;

                if (0 != dlinfo(_hndl, RTLD_DI_LMID, &id))
                    symbolError();
                else
                    _scope = id;
            } else
                _scope = s;
        }

        return !_has_error;
    }

    std::string LinuxScopedBackend::getPath() const noexcept {
        return LinuxBackend::getPath();
    }

    bool LinuxScopedBackend::hasSymbol(std::string const &name) noexcept {
        return LinuxBackend::hasSymbol(name);
    }

    LinuxScopedBackend::SymAddr LinuxScopedBackend::getSymbol(std::string const &name) noexcept {
        return LinuxBackend::getSymbol(name);
    }

    bool LinuxScopedBackend::hasError() const noexcept {
        return LinuxBackend::hasError();
    }

    std::string LinuxScopedBackend::getLastError() const noexcept {
        return LinuxBackend::getLastError();
    }

    LinuxScopedBackend::Scope LinuxScopedBackend::getScope() const noexcept {
        return _scope;
    }
}

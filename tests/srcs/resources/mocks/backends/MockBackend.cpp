
#include <iostream>
#include "./MockBackend.hpp"

namespace tests::mocks::backends {
    MockBackend::fail_t const MockBackend::dont_fail = {false, std::string()};

    MockBackend::fail_t MockBackend::fail_with(std::string const &reason) {
        return {true, reason};
    }

    MockBackend::MockBackend(std::string const &path, fail_t const &fail, std::initializer_list<ptrmap_v> l)
    : _path(path), _map(l), _has_error(false) {
        _has_error = fail.first;
        _last_error = fail.second;
    }

    MockBackend::MockBackend(MockBackend &&m) noexcept
    : _path(std::move(m._path)), _map(std::move(m._map)), _has_error(false), _fail_next(dont_fail) {
    }

    MockBackend &MockBackend::operator=(MockBackend &&rhs) noexcept {
        _path = std::move(rhs._path);
        _map = std::move(rhs._map);

        _last_error = std::string();
        _has_error = false;
        return *this;
    }

    bool MockBackend::reset(std::string const &path, std::initializer_list<ptrmap_v> l, fail_t const &fail) noexcept {
        _has_error = fail.first;
        _last_error = fail.second;

        if (!fail.first) {
            _path = path;
            _map = l;
        }

        return _has_error;
    }

    bool    MockBackend::hasSymbol(std::string const &name) const noexcept {
        resetLastError();

        auto search = _map.find(name);

        return (search != _map.end());
    }

    MockBackend::SymAddr MockBackend::getSymbol(std::string const &name) noexcept {
        SymAddr ret = nullptr;

        resetLastError();

        if (hasSymbol(name) && !_fail_next.first)
            ret = _map[name];
        else {
            _has_error = true;

            if (_fail_next.first) {
                _last_error = _path + _fail_next.second + " when looking for symbol " + name;
                _fail_next = dont_fail;
            } else {
                _last_error = _path + ": Could not find symbol: " + name;
            }
        }

        return ret;
    }

    bool MockBackend::hasError() const {
        return _has_error;
    }

    std::string MockBackend::getLastError() const {
        return _last_error;
    }

    std::string MockBackend::getPath() const {
        return _path;
    }

    void MockBackend::setNextError(MockBackend::fail_t fail){
        _fail_next = fail;
    }

    void *&     MockBackend::operator[](std::string const &k) {
        return _map[k];
    }

    void MockBackend::resetLastError() const {
        _has_error = false;
        _last_error = std::string();
    }
}

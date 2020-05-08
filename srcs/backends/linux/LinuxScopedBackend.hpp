/**
** \file LinuxScopedBackend.hpp
**
** \author Phantomas <phantomas@phantomas.xyz>
** \date Created on: 2020-04-28 16:37
** \date Last update: 2020-04-28 16:37
** \copyright GNU Lesser Public Licence v3
*/

#ifndef LinuxScopedBackend_hpp_
#define LinuxScopedBackend_hpp_

#include <string>

#include <dlfcn.h>

#include "./OpenFlags.hpp"
#include "./LinuxBackend.hpp"

namespace clonixin::dynamicloader::backends::_linux {
    using namespace std::string_literals;

    class LinuxScopedBackend;

    namespace _internals {
        class Scope {
            Lmid_t _id;
            public:
                constexpr Scope(Scope const &oth) noexcept : _id(oth._id) {}

                constexpr bool operator==(Scope const &rhs) const { return _id == rhs._id; }
                constexpr bool operator!=(Scope const &rhs) const { return !(*this == rhs); }

                constexpr Scope &operator=(Scope const &rhs) {
                    if (this != std::addressof(rhs))
                        _id = rhs._id;

                    return *this;
                }

                constexpr Scope &operator=(Scope && rhs) {
                    if (this != std::addressof(rhs))
                        _id = std::move(rhs._id);

                    return *this;
                }
            private:
            constexpr Scope(Lmid_t id) noexcept : _id(id) {}
            constexpr Lmid_t get() const { return _id; }

            friend LinuxScopedBackend;
        };
    }


    class LinuxScopedBackend : protected LinuxBackend {
        public:
            using Scope = _internals::Scope;
            using SymAddr = LinuxBackend::SymAddr;
            static constexpr Scope BaseScope = LM_ID_BASE;
            static constexpr Scope NewScope = LM_ID_NEWLM;

            LinuxScopedBackend(std::string const &path, Scope s, OpenFlags f = OpenFlags::Default);
            LinuxScopedBackend(LinuxScopedBackend const &) = delete;
            LinuxScopedBackend(LinuxScopedBackend &&oth) noexcept;

            virtual ~LinuxScopedBackend();

            LinuxScopedBackend &operator=(LinuxScopedBackend const &) = delete;
            LinuxScopedBackend &operator=(LinuxScopedBackend &&rhs) noexcept;

            bool reset(std::string const &path, Scope s, OpenFlags f = OpenFlags::Default) noexcept;

            [[nodiscard]]
            std::string getPath() const noexcept;

            [[nodiscard]]
            bool hasSymbol(std::string const &name) noexcept;
            [[nodiscard]]
            SymAddr getSymbol(std::string const &name) noexcept;

            [[nodiscard]]
            bool hasError() const noexcept;
            std::string getLastError() const noexcept;

            Scope getScope() const noexcept;
        private:
            Scope _scope;
    };
}

#endif

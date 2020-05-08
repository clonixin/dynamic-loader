/**
** \file LinuxBackend.hpp
** Linux default backend header.
**
** \author Phantomas <phantomas@phantomas.xyz>
** \date Created on: 2020-04-28 16:36
** \date Last update: 2020-04-29 02:23
** \copyright GNU Lesser Public Licence v3
*/

#ifndef LinuxBackend_hpp_
#define LinuxBackend_hpp_

#include <string>

#include <dlfcn.h>

#include "./OpenFlags.hpp"

namespace clonixin::dynamicloader::backends::_linux {
    using namespace std::string_literals;

    class LinuxBackend {
        inline static const std::string InternalPath = "Program Internal"s;
#ifdef __USE_GNU
        inline static void * const GlobHndl = RTLD_DEFAULT;
        inline static const std::string GlobPath = "Global Scope"s;
        inline static void * const NextHndl = RTLD_NEXT;
        inline static const std::string NextPath = "Next Symbol"s;

        public:
            static LinuxBackend InternalSymbolBackend(OpenFlags f);
            static LinuxBackend GlobSymbolBackend();
            static LinuxBackend NextSymbolBackend();
#endif

        public:

            using SymAddr = void *;
        public:
            LinuxBackend(std::string const &path, OpenFlags f = OpenFlags::Default) noexcept;
            LinuxBackend(LinuxBackend const &) = delete;
            LinuxBackend(LinuxBackend &&oth) noexcept;

            virtual ~LinuxBackend();

            LinuxBackend &operator=(LinuxBackend const &) = delete;
            LinuxBackend &operator=(LinuxBackend &&rhs) noexcept;

            bool reset(std::string const &path, OpenFlags f = OpenFlags::Default) noexcept;

            [[nodiscard]]
            std::string getPath() const noexcept;

            [[nodiscard]]
            bool hasSymbol(std::string const &name) noexcept;
            [[nodiscard]]
            SymAddr getSymbol(std::string const &name) noexcept;

            [[nodiscard]]
            bool hasError() const noexcept;
            std::string getLastError() const noexcept;

        protected:
            LinuxBackend() noexcept;
            void resetError();
            void symbolError();

        private:
            LinuxBackend(std::string const &path, void *hndl) noexcept;

        protected:
            std::string _path;

            void * _hndl;
            bool _has_error;
            std::string _err_str;
    };
}

#endif

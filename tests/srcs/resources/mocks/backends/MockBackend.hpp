#ifndef MOCKS_BACKENDS_MOCKBACKEND_HPP__
#define MOCKS_BACKENDS_MOCKBACKEND_HPP__

#include <unordered_map>

namespace tests::mocks::backends {
    class MockBackend {
            using ptrmap_t = std::unordered_map<std::string, void *>;
            using ptrmap_v = ptrmap_t::value_type;
        public:
            using list_t = std::initializer_list<ptrmap_v>;
            using fail_t = std::pair<bool, std::string>;
            using SymAddr = void *;

            static fail_t const dont_fail;
            static fail_t fail_with(std::string const &reason);

            MockBackend(std::string const &path, fail_t const &fail = dont_fail, std::initializer_list<ptrmap_v> l = {});
            MockBackend(MockBackend &&m) noexcept;
            ~MockBackend() {}

            MockBackend &operator=(MockBackend const &) = delete;
            MockBackend &operator=(MockBackend &&) noexcept;

            bool reset(std::string const &path, std::initializer_list<ptrmap_v> l, fail_t const &fail = dont_fail) noexcept;

            bool        hasSymbol(std::string const &name) const noexcept;
            SymAddr     getSymbol(std::string const &name) noexcept;

            bool        hasError() const;
            std::string getLastError() const;
            std::string getPath() const;

            void        setNextError(fail_t fail);
            void *&     operator[](std::string const &key);// {return _map[key];}

        private:
            void resetLastError() const;
        private:
            ptrmap_t _map;
            std::string _path;
            mutable std::string _last_error;
            mutable bool _has_error;
            std::pair<bool, std::string> _fail_next;
    };
} // namespace clonixin::dynamicloader::back

#endif

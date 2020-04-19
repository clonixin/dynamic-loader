#ifndef RES_SINGLETON_HPP__
#define RES_SINGLETON_HPP__

namespace tests::resources {
    class Singleton {
        public:
            static Singleton &getInstance(int init_v = 0);

            Singleton(Singleton const &) = delete;
            Singleton(Singleton &&) = delete;

            Singleton &operator =(Singleton const &) = delete;
            Singleton &operator =(Singleton &&) = delete;

            bool operator==(Singleton const &rhs) const {
                return this == &rhs && _val == rhs._val;
            }

            int operator++() { return ++_val; }

            int getVal() const;
        private:
            Singleton(int val) : _val(val) {}
            int _val;
    };
}

#endif

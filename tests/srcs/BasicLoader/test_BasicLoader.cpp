#include <criterion/criterion.h>
#include <string>

#include "BasicLoader/BasicLoader.hpp"
#include "../resources/resources.h"
#include "../resources/mocks.h"

/*
** This is there because of a bug in Criterion v2.3.3, where cr_assert_throw
** does not fail if the statement does not throw anything.
*/
#define will_throw(x) x; throw std::runtime_error("Statement did not throw.")

namespace cd = clonixin::dynamicloader;
namespace cde = clonixin::dynamicloader::exceptions;

namespace ts = tests;

using list_t = tmb::MockBackend::list_t;
using fail_t = tmb::MockBackend::fail_t;

using namespace std::string_literals;

class Int {
        int _val;
        public:
        Int(int v): _val(v) {}
        int getVal() {return _val;}
    };

static int integer = 10;
static int integers[5] = {1, 2, 3, 4, 5};
static float floating = 0.6;
static struct { int val; } anon_s = {.val = 10};
static Int i(25);

static tr::Singleton &s = tr::Singleton::getInstance(10);
static tr::Copyable c(10);
static tr::Movable m(10);
static tr::CopyableAndMovable cm(10);

tmb::MockBackend setup(fail_t fail = tmb::MockBackend::dont_fail) {
#define EZ_CREATE(x) #x, &x
    return tmb::MockBackend("PATH"s, fail, list_t{
        {EZ_CREATE(integer)},
        {EZ_CREATE(integers)},
        {EZ_CREATE(floating)},
        {EZ_CREATE(anon_s)},
        {EZ_CREATE(i)},
        {EZ_CREATE(s)},
        {EZ_CREATE(c)},
        {EZ_CREATE(m)},
        {EZ_CREATE(cm)},
        {"NULL", NULL},
        {"nullptr", nullptr}
    });
#undef EZ_CREATE
}

TestSuite(BasicLoaderTests, .fini = ts::resetFlags);

Test(BasicLoaderTests, Instantiation, .description = "Try to instantiate a BasicLoader "
        "templated on MockBackend") {
    auto bdl = cd::BasicLoader<tmb::MockBackend>("PATH"s, tmb::MockBackend::dont_fail, list_t{});
}

Test(BasicLoaderTests, InstatiationException, .description = "Try to instantiate"
        " a BasicLoader templated on MockBacked. The backend cannot be instant"
        "iated, so BasicLoader should throw an exception") {
    cr_assert_throw(will_throw(auto bdl = cd::BasicLoader<tmb::MockBackend>(
                    "PATH"s, fail_t{true, "fail"})), cde::DLException<cde::Type::Open>);
}

/* Testing address returning functions */
Test(BasicLoaderTests, GetSymbolPointers, .description = "Instantiate a BasicLoader, "
        "then retrieve symbols using pointers.") {
    auto bdl = cd::BasicLoader<tmb::MockBackend>(setup());

    cr_assert(&integer == bdl.getSymbol<std::remove_reference_t<decltype(integer)> *>("integer"s));
    cr_assert(&integers == bdl.getSymbol<std::remove_reference_t<decltype(integers)> *>("integers"s));
    cr_assert(&floating == bdl.getSymbol<std::remove_reference_t<decltype(floating)> *>("floating"s));
    cr_assert(&anon_s == bdl.getSymbol<std::remove_reference_t<decltype(anon_s)> *>("anon_s"s));
    cr_assert(&i == bdl.getSymbol<std::remove_reference_t<decltype(i)> *>("i"s));
    cr_assert(&s == bdl.getSymbol<std::remove_reference_t<decltype(s)> *>("s"s));
    cr_assert(&c == bdl.getSymbol<std::remove_reference_t<decltype(c)> *>("c"s));
    cr_assert(&m == bdl.getSymbol<std::remove_reference_t<decltype(m)> *>("m"s));
    cr_assert(&cm == bdl.getSymbol<std::remove_reference_t<decltype(cm)> *>("cm"s));
    cr_assert(NULL == bdl.getSymbol<std::remove_reference_t<decltype(NULL)> *>("NULL"s));
    cr_assert(nullptr == bdl.getSymbol<std::remove_reference_t<decltype(nullptr)> *>("nullptr"s));
}

Test(BasicLoaderTests, GetSymbolPointersException, .description = "Instantiate "
        "a BasicLoader, then get an unknown symbol, which should throw an exception.") {
    auto bdl = cd::BasicLoader(setup());

    cr_assert_throw(will_throw((void)bdl.getSymbol<int *>("toto"s)), cde::DLException<cde::Type::LoadSym>);
}

Test(BasicLoaderTests, TryGetSymbolPointers, .description = "Instantiate a BasicLoader, "
        "then retrieve symbols using pointers.") {
    auto bdl = cd::BasicLoader<tmb::MockBackend>(setup());

    cr_assert(&integer == bdl.tryGetSymbol<std::remove_reference_t<decltype(integer)> *>("integer"s));
    cr_assert(&integers == bdl.tryGetSymbol<std::remove_reference_t<decltype(integers)> *>("integers"s));
    cr_assert(&floating == bdl.tryGetSymbol<std::remove_reference_t<decltype(floating)> *>("floating"s));
    cr_assert(&anon_s == bdl.tryGetSymbol<std::remove_reference_t<decltype(anon_s)> *>("anon_s"s));
    cr_assert(&i == bdl.tryGetSymbol<std::remove_reference_t<decltype(i)> *>("i"s));
    cr_assert(&s == bdl.tryGetSymbol<std::remove_reference_t<decltype(s)> *>("s"s));
    cr_assert(&c == bdl.tryGetSymbol<std::remove_reference_t<decltype(c)> *>("c"s));
    cr_assert(&m == bdl.tryGetSymbol<std::remove_reference_t<decltype(m)> *>("m"s));
    cr_assert(&cm == bdl.tryGetSymbol<std::remove_reference_t<decltype(cm)> *>("cm"s));
    cr_assert((void *)NULL == bdl.tryGetSymbol<std::remove_reference_t<decltype(NULL)> *>("NULL"s));
    cr_assert(nullptr == bdl.tryGetSymbol<std::remove_reference_t<decltype(nullptr)> *>("nullptr"s));
}

Test(BasicLoaderTests, TryGetInvalidSymbolPointers, .description = "Instantiate"
        " a BasicLoader, then try to retrieve a non-existing symbol. It should "
        "return nullopt.") {
    auto bdl = cd::BasicLoader<tmb::MockBackend>(setup());
    std::string err_out;
    cde::Type err_type;

    cr_assert_not(bdl.tryGetSymbol<int *>("toto"), "The returned optional is not empty.");
    cr_assert_not(bdl.tryGetSymbol<int *>("toto", err_type, err_out), "The returned optional is not empty.");
    cr_assert_eq(err_type, cde::Type::LoadSym, "The returned error is not of type cde::Type::LoadSym.");
    cr_assert_not(err_out.empty(), "Error string is empty but should not.");
}
/* !Testing address returning functions */

/* Testing lvalue reference returning functions */
Test(BasicLoaderTests, GetByLValueRef, .description = "Instantiate a BasicLoader,"
        " then retrieve a Singleton by lvalue reference.") {
    auto bdl = cd::BasicLoader(setup());

    tr::Singleton &s2 = bdl.getSymbol<tr::Singleton &>("s"s);
    ++s2;

    cr_assert_eq(s2, s, "Retrieved another singleton, somehow.");
}

Test(BasicLoaderTests, GetArrayByLValueRef, .description = "Instantiate a BasicLoader,"
        " then retrieve an array by lvalue reference.") {
    auto bdl = cd::BasicLoader(setup());

    int (&i)[5] = bdl.getSymbol<int (&)[5]>("integers"s);

    for (int j = 0; j < 5; ++j) {
        cr_expect_eq(i[j], integers[j], "Failed at index %d. i[{index}] = %d, integers[{index}] = %d",
            j, i[j], integers[j]
        );
    }
}

Test(BasicLoaderTests, GetByLValueRefLoadSymException, .description = "Instantiate a BasicLoader, "
        "the try to retrieve a symbol that does not exists. This should throw a LoadSym typed exception.") {
    auto bdl = cd::BasicLoader(setup());

    cr_assert_throw(will_throw((void)bdl.getSymbol<tr::Copyable &>("toto")), cde::DLException<cde::Type::LoadSym>)
}

Test(BasicLoaderTests, GetByLValueRefNullSymException, .description = "Instantiate a BasicLoader, "
        "then try to retrieve a symbol whose address is NULL or nullptr. This should throw a NullSym typed exception.") {
    auto bdl = cd::BasicLoader(setup());

    cr_assert_throw(will_throw((void)bdl.getSymbol<tr::Copyable &>("NULL")), cde::DLException<cde::Type::NullSym>)
    cr_assert_throw(will_throw((void)bdl.getSymbol<tr::Copyable &>("nullptr")), cde::DLException<cde::Type::NullSym>)
}

Test(BasicLoaderTests, TryGetByLValueRef, .description = "Instantiate a BasicLoader,"
        " then try to retrieve a Singleton by lvalue reference.") {
    auto bdl = cd::BasicLoader(setup());

    auto opt_s = bdl.tryGetSymbol<tr::Singleton &>("s"s);
    cr_assert_neq(opt_s, std::nullopt, "Nullopt received.");

    tr::Singleton &s2 = opt_s.value();
    ++s2;

    cr_assert_eq(s2, s, "Retrieved another singleton, somehow.");
}

Test(BasicLoaderTests, TryGetArrayByLValueRef, .description = "Instantiate a BasicLoader,"
        " then retrieve symbols value by lvalue reference using tryGetValue.") {
    auto bdl = cd::BasicLoader(setup());

    auto opt_i = bdl.tryGetSymbol<int (&)[5]>("integers"s);
    cr_assert_neq(opt_i, std::nullopt, "Nullopt received.");

    int (&i)[5] = opt_i.value();

    for (int j = 0; j < 5; ++j) {
        cr_expect_eq(i[j], integers[j], "Failed at index %d. i[{index}] = %d, integers[{index}] = %d",
            j, i[j], integers[j]
        );
    }
}

Test(BasicLoaderTests, TryGetByLValueRefLoadSymError, .description = "Instantiate a BasicLoader, "
        "them try to retrieve a symbol that does not exists. It should return nullopt, with an error "
        "type of LoadSym."
    ) {
    auto bdl = cd::BasicLoader(setup());
    std::string err_out;
    cde::Type err_type;

    cr_assert_eq(bdl.tryGetSymbol<tr::Singleton &>("toto"s), std::nullopt, "Non empty nullopt received.");
    cr_assert_eq(bdl.tryGetSymbol<tr::Singleton &>("toto"s, err_type, err_out), std::nullopt, "Non empty optional received.");
    cr_assert_not(err_out.empty(), "err_out is not empty.");
    cr_assert_eq(err_type, cde::Type::LoadSym, "err_type is not cde::Type::LoadSym");
}

Test(BasicLoaderTests, TryGetByLValueRefNullSymError, .description = "Instantiate a BasicLoader, "
        "them try to retrieve a symbol whose address is NULL or nullptr. It should return nullopt, with"
        " an error type of NullSym.") {
    auto bdl = cd::BasicLoader(setup());
    std::string err_out;
    cde::Type err_type;

    cr_assert_eq(bdl.tryGetSymbol<tr::Singleton &>("NULL"s), std::nullopt, "Non empty nullopt received.");
    cr_assert_eq(bdl.tryGetSymbol<tr::Singleton &>("NULL"s, err_type, err_out), std::nullopt, "Non empty nullopt received.");
    cr_assert_not(err_out.empty(), "err_out is not empty.");
    cr_assert_eq(err_type, cde::Type::NullSym, "err_type is not cde::Type::NullSym");

    std::string err_out2;
    cde::Type err_type2;
    cr_assert_eq(bdl.tryGetSymbol<tr::Singleton &>("nullptr"s), std::nullopt, "Non empty nullopt received.");
    cr_assert_eq(bdl.tryGetSymbol<tr::Singleton &>("nullptr"s, err_type2, err_out2), std::nullopt, "Non empty nullopt received.");
    cr_assert_not(err_out2.empty(), "err_out is not empty.");
    cr_assert_eq(err_type2, cde::Type::NullSym, "err_type is not cde::Type::NullSym");
}
/* !Testing lvalue reference returning functions */

/* Testing copy semantics getters */
Test(BasicLoaderTests, GetValueByCopy, .description = "Instantiate a BasicLoader,"
        " then retrieve symbols value by copy.") {
    auto bdl = cd::BasicLoader(setup());

    tr::Copyable c2 = bdl.getSymbol<std::remove_reference_t<decltype(c)>>("c"s);
    cr_assert(ts::copyCalled());
}

Test(BasicLoaderTests, GetValueByCopyLoadSymException, .description = "Instantiate a BasicLoader, "
        "the try to retrieve a symbol that does not exists. This should throw a LoadSym typed exception.") {
    auto bdl = cd::BasicLoader(setup());

    cr_assert_throw(will_throw((void)bdl.getSymbol<tr::Copyable>("toto")), cde::DLException<cde::Type::LoadSym>)
}

Test(BasicLoaderTests, GetValueByCopyNullSymException, .description = "Instantiate a BasicLoader, "
        "the try to retrieve a symbol that does not exists. This should throw a NullSym typed exception.") {
    auto bdl = cd::BasicLoader(setup());

    cr_assert_throw(will_throw((void)bdl.getSymbol<tr::Copyable>("NULL")), cde::DLException<cde::Type::NullSym>)
    cr_assert_throw(will_throw((void)bdl.getSymbol<tr::Copyable>("nullptr")), cde::DLException<cde::Type::NullSym>)
}

Test(BasicLoaderTests, TryGetValueByCopy, .description = "Instantiate a BasicLoader,"
        " then retrieve a symbol value by copy.") {
    auto bdl = cd::BasicLoader(setup());

    auto opt_c = bdl.tryGetSymbol<std::remove_reference_t<decltype(c)>>("c"s);
    cr_assert_neq(opt_c, std::nullopt, "Optional does not contains value");
    cr_assert(ts::copyCalled());
}

Test(BasicLoaderTests, TryGetValueByCopyLoadSymError, .description = "Instantiate a BasicLoader, "
        "them try to retrieve a symbol that does not exists. It should return nullopt, with an error "
        "type of LoadSym."
    ) {
    auto bdl = cd::BasicLoader(setup());
    std::string err_out;
    cde::Type err_type;

    cr_assert_eq(bdl.tryGetSymbol<tr::Copyable>("toto"s), std::nullopt, "Non empty nullopt received.");
    cr_assert_eq(bdl.tryGetSymbol<tr::Copyable>("toto"s, err_type, err_out), std::nullopt, "Non empty optional received.");
    cr_assert_not(err_out.empty(), "err_out is not empty.");
    cr_assert_eq(err_type, cde::Type::LoadSym, "err_type is not cde::Type::LoadSym");
}

Test(BasicLoaderTests, TryGetValueByCopyNullSymError, .description = "Instantiate a BasicLoader, "
        "them try to retrieve a symbol whose address is NULL or nullptr. It should return nullopt, with"
        " an error type of NullSym.") {
    auto bdl = cd::BasicLoader(setup());
    std::string err_out;
    cde::Type err_type;

    cr_assert_eq(bdl.tryGetSymbol<tr::Copyable>("NULL"s), std::nullopt, "Non empty nullopt received.");
    cr_assert_eq(bdl.tryGetSymbol<tr::Copyable>("NULL"s, err_type, err_out), std::nullopt, "Non empty nullopt received.");
    cr_assert_not(err_out.empty(), "err_out is not empty.");
    cr_assert_eq(err_type, cde::Type::NullSym, "err_type is not cde::Type::NullSym");

    std::string err_out2;
    cde::Type err_type2;
    cr_assert_eq(bdl.tryGetSymbol<tr::Copyable &>("nullptr"s), std::nullopt, "Non empty nullopt received.");
    cr_assert_eq(bdl.tryGetSymbol<tr::Copyable &>("nullptr"s, err_type2, err_out2), std::nullopt, "Non empty nullopt received.");
    cr_assert_not(err_out2.empty(), "err_out is not empty.");
    cr_assert_eq(err_type2, cde::Type::NullSym, "err_type is not cde::Type::NullSym");
}
/* !Testing copy semantics getters */

/* Testing move semantics getters */
Test(BasicLoaderTests, GetValueByMove, .description = "Instantiate a BasicLoader, "
        "then retrieve symbols value using move semantic.") {
    auto bdl = cd::BasicLoader(setup());

    tr::Movable m2 = bdl.getSymbol<std::remove_reference_t<decltype(m)>>("m"s);
    cr_assert(ts::moveCalled());
}

Test(BasicLoaderTests, GetValueByMoveLoadSymException, .description = "Instantiate a BasicLoader, "
        "the try to retrieve a symbol that does not exists. This should throw a LoadSym typed exception.") {
    auto bdl = cd::BasicLoader(setup());

    cr_assert_throw(will_throw((void)bdl.getSymbol<tr::Movable>("toto")), cde::DLException<cde::Type::LoadSym>)
}

Test(BasicLoaderTests, GetValueByMoveNullSymException, .description = "Instantiate a BasicLoader, "
        "the try to retrieve a symbol that does not exists. This should throw a NullSym typed exception.") {
    auto bdl = cd::BasicLoader(setup());

    cr_assert_throw(will_throw((void)bdl.getSymbol<tr::Movable>("NULL")), cde::DLException<cde::Type::NullSym>)
    cr_assert_throw(will_throw((void)bdl.getSymbol<tr::Movable>("nullptr")), cde::DLException<cde::Type::NullSym>)
}

Test(BasicLoaderTests, TryGetValueByMove, .description = "Instantiate a BasicLoader,"
        " then retrieve a symbol value by copy.") {
    auto bdl = cd::BasicLoader(setup());

    auto opt_c = bdl.tryGetSymbol<tr::Movable>("m"s);
    cr_assert_neq(opt_c, std::nullopt, "Optional does not contains value");
    cr_assert(ts::moveCalled());
}

Test(BasicLoaderTests, TryGetValueByMoveLoadSymError, .description = "Instantiate a BasicLoader, "
        "them try to retrieve a symbol that does not exists. It should return nullopt, with an error "
        "type of LoadSym."
    ) {
    auto bdl = cd::BasicLoader(setup());
    std::string err_out;
    cde::Type err_type;

    cr_assert_eq(bdl.tryGetSymbol<tr::Movable>("toto"s), std::nullopt, "Non empty nullopt received.");
    cr_assert_eq(bdl.tryGetSymbol<tr::Movable>("toto"s, err_type, err_out), std::nullopt, "Non empty optional received.");
    cr_assert_not(err_out.empty(), "err_out is not empty.");
    cr_assert_eq(err_type, cde::Type::LoadSym, "err_type is not cde::Type::LoadSym");
}

Test(BasicLoaderTests, TryGetValueByMoveNullSymError, .description = "Instantiate a BasicLoader, "
        "them try to retrieve a symbol whose address is NULL or nullptr. It should return nullopt, with"
        " an error type of NullSym.") {
    auto bdl = cd::BasicLoader(setup());
    std::string err_out;
    cde::Type err_type;

    cr_assert_eq(bdl.tryGetSymbol<tr::Movable>("NULL"s), std::nullopt, "Non empty nullopt received.");
    cr_assert_eq(bdl.tryGetSymbol<tr::Movable>("NULL"s, err_type, err_out), std::nullopt, "Non empty nullopt received.");
    cr_assert_not(err_out.empty(), "err_out is not empty.");
    cr_assert_eq(err_type, cde::Type::NullSym, "err_type is not cde::Type::NullSym");

    std::string err_out2;
    cde::Type err_type2;
    cr_assert_eq(bdl.tryGetSymbol<tr::Movable &>("nullptr"s), std::nullopt, "Non empty nullopt received.");
    cr_assert_eq(bdl.tryGetSymbol<tr::Movable &>("nullptr"s, err_type2, err_out2), std::nullopt, "Non empty nullopt received.");
    cr_assert_not(err_out2.empty(), "err_out is not empty.");
    cr_assert_eq(err_type2, cde::Type::NullSym, "err_type is not cde::Type::NullSym");
}

Test(BasicLoaderTests, MovePrecedence, .description = "Instantiate a BasicLoader, "
        "then retrieve symbols value. Check that move semantic is used instead of "
        "copy semantic, when available.") {
    auto bdl = cd::BasicLoader(setup());

    tr::CopyableAndMovable cm2 = bdl.getSymbol<std::remove_reference_t<decltype(cm2)>>("m"s);
    cr_assert_not(ts::copyCalled());
    cr_assert(ts::moveCalled());

    ts::resetFlags();
    std::optional<tr::CopyableAndMovable> cm3 = bdl.tryGetSymbol<std::remove_reference_t<decltype(cm2)>>("m"s);
}
/* !Testing move semantics getters */

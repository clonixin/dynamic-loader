/**
** \file utils/sfinae.hpp
** Header file defining some types and value used for SFINAE.
**
** \author phantomas <phantomas@phantomas.xyz>
** \date Creation: 2020-04-19 01:58
** \date Last update: 2020-04-28 16:31
** \copyright GNU Lesser Public Licence v3
*/

#ifndef utils_sfinae_hpp_
#define utils_sfinae_hpp_

#include <type_traits>

namespace clonixin::dynamicloader {
    /**
    ** \brief SFINAE utilities and short aliases.
    */
    namespace {
        /**
        ** \brief SFINAE utility for detecting pointer types.
        **
        ** isptr::value is true, if T is a pointer.
        ** This type is an alias on std::is_pointer<T>.
        **
        ** \tparam T The type to check.
        */
        template <typename T>
        using isptr = std::is_pointer<T>;

        /**
        ** \brief SFINAE utility for detecting lvalue reference types..
        **
        ** islref::value is true, if T is a lvalue reference.
        ** This type is an alias on std::is_lvalue_reference<T>.
        **
        ** \tparam T The type to check.
        */
        template <typename T>
        using islref = std::is_lvalue_reference<T>;

        /**
        ** \brief SFINAE utility for detecting movable types.
        **
        ** ismovable::value is true, if T is movable, that is, if T is either
        ** move constructible or move assignable.
        **
        ** \tparam T The type to check.
        */
        template <typename T>
        using ismovable = std::disjunction<std::is_move_constructible<T>, std::is_move_assignable<T>>;

        /**
        ** \brief SFINAE utility for detecting copyable types.
        **
        ** iscopyable::value is true, if T is copyable, that is, if T is either
        ** copy constructible or copy assignable.
        **
        ** \tparam T The type to check.
        */
        template <typename T>
        using iscopyable = std::disjunction<std::is_copy_constructible<T>, std::is_copy_assignable<T>>;

        /**
        ** \brief SFINAE utility, true if T is a pointer.
        **
        ** \tparam T The type to check.
        */
        template <typename T>
        inline constexpr bool isptr_v = isptr<T>::value;

        /**
        ** \brief SFINAE utility, true if T is a lvalue reference.
        **
        ** \tparam T The type to check.
        */
        template <typename T>
        inline constexpr bool islref_v = islref<T>::value;

        /**
        ** \brief SFINAE utility, used to detect movable types.
        **
        ** This bool is true if T is movable and is neither a pointer nor a
        ** lvalue reference.
        **
        ** \tparam T The type to check.
        */
        template <typename T>
        inline constexpr bool ismovable_v = std::conjunction_v<
            ismovable<T>,
            std::negation<
                std::disjunction<isptr<T>, islref<T>>
            >
        >;

        /**
        ** \brief SFINAE utility, used to detect copyable types
        **
        ** This bool is true if T is copyable and is neither a pointer nor a
        ** lvalue reference, nor a movable type.
        **
        ** \tparam T The type to check.
        */
        template <typename T>
        inline constexpr bool iscopyable_v = std::conjunction_v<
            iscopyable<T>,
            std::negation<
                std::disjunction<ismovable<T>, isptr<T>, islref<T>>
            >
        >;

        /**
        ** \brief SFINAE utility, used to detect erroneous types.
        **
        ** This boolean is true if T is neither a pointer, nor a lvalue
        ** reference, nor movable, nor copyable. Effectively serving as a
        ** catch-all clause for T.
        **
        ** \tparam T The type to check.
        */
        template <typename T>
        inline constexpr bool iserror_v = std::negation_v<
            std::disjunction<isptr<T>, ismovable<T>, iscopyable<T>, islref<T>>
        >;

        /**
        ** \brief SFINAE utility to enable a function if T is a pointer type.
        **
        ** \tparam T The type to check.
        */
        template <typename T>
        using ifptr_t = std::enable_if_t<isptr_v<T>, T>;

        /**
        ** \brief SFINAE utility to enable a function if T is a lvalue
        ** reference type.
        **
        ** \tparam T The type to check.
        */
        template <typename T>
        using iflref_t = std::enable_if_t<islref_v<T>, T>;

        /**
        ** \brief SFINAE utility to enable a function if T is a movable type.
        **
        ** \tparam T The type to check.
        */
        template <typename T>
        using ifmove_t = std::enable_if_t<ismovable_v<T>, T>;

        /**
        ** \brief SFINAE utility to enable a function if T is a copyable type.
        **
        ** \tparam T The type to check.
        */
        template <typename T>
        using ifcopy_t = std::enable_if_t<iscopyable_v<T>, T>;

        /**
        ** \brief SFINAE utility to enable a fallback for erroneous types.
        **
        ** \tparam T The type to check.
        */
        template <typename T>
        using iferror_t = std::enable_if_t<iserror_v<T>, T>;

        /**
        ** \brief Convenience type declaration to return a reference_wrapper
        ** when a reference type is requested.
        **
        ** Alias on std::reference_wrapper<std::remove_reference_t<T>>.
        **
        ** \tparam T The reference type T to that we'd like to wrap.
        */
        template <typename T>
        using refw = std::reference_wrapper<std::remove_reference_t<T>>;
    }
}

#endif

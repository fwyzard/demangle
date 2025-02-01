/*
 * Copyright 2025 Andrea Bocci, Simeon Ehrig
 * SPDX-License-Identifier: MPL-2.0
 */

/*
 * C++20 compile-time demangled type name, based on
 *   https://www.reddit.com/r/cpp/comments/lfi6jt/finally_a_possibly_portable_way_to_convert_types/
 *   https://rodusek.com/posts/2021/03/09/getting-an-unmangled-type-name-at-compile-time/
 */

#pragma once

#include <array>
#include <source_location>
#include <string_view>
#include <utility>

namespace
{

    // Use a fundamental type to detect the prefix and suffix, because MSVC includes the class, struct, or union
    // keyword in the type signature.
    using test_pattern_type = double;

    constexpr std::string_view test_pattern_name("double");

    // Generate a function name with full type signature that includes the user-provided type name.
    // Use the compiler-specific extensions for known compilers because nvc++ does not include the full type signature
    // in std::source_location::current().function_name() .
    template<typename T>
    consteval std::string_view get_function_name()
    {
#if defined(__GNUC__) || defined(__clang__) || defined(__PGI) || defined(__NVCOMPILER)
        // gcc, clang, PGI, nvc++
        return __PRETTY_FUNCTION__;
#elif defined(_MSC_VER)
        // MSVC
        return __FUNCSIG__;
#else
        // unknown compiler, use the c++20 standard approach
        return std::source_location::current().function_name();
#endif
    }

    consteval size_t prefix_size()
    {
        return get_function_name<test_pattern_type>().find(test_pattern_name);
    }

    consteval size_t suffix_size()
    {
        return get_function_name<test_pattern_type>().size() - prefix_size() - test_pattern_name.size();
    }

    template<typename T>
    consteval auto demangle_as_substr()
    {
        constexpr std::string_view text = get_function_name<T>();
        constexpr size_t start = prefix_size();
        constexpr size_t end = text.size() - suffix_size();
        static_assert(start < end);
        constexpr size_t length = end - start;
        return text.substr(start, length);
    }

    template<std::size_t... Idxs>
    consteval auto substring_as_array(std::string_view str, std::index_sequence<Idxs...>)
    {
        // Add null termination
        return std::array{str[Idxs]..., '\0'};
    }

    template<typename T>
    consteval auto demangle_as_array()
    {
        constexpr std::string_view name = demangle_as_substr<T>();
        return substring_as_array(name, std::make_index_sequence<name.size()>{});
    }

    template<typename T>
    inline constexpr auto storage = demangle_as_array<T>();

    template<typename T>
    consteval std::string_view demangle()
    {
        return std::string_view{storage<T>.data(), storage<T>.size()};
    }

} // namespace

template<typename T>
inline constexpr std::string_view demangled = demangle<T>();

/*
    Licensed under the MIT License <http://opensource.org/licenses/MIT>.
    SPDX-License-Identifier: MIT
    Copyright (c) 2020 - 2020 Daniil Dudkin.

    Permission is hereby  granted, free of charge, to any  person obtaining a copy
    of this software and associated  documentation files (the "Software"), to deal
    in the Software  without restriction, including without  limitation the rights
    to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
    copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
    IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
    FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
    AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
    LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#define CATCH_CONFIG_MAIN

#include <algorithm>
#include <catch.hpp>
#include <string>

#include "fixed_string.hpp"

constexpr std::size_t default_size = 15;

using namespace fixstr;

using fs = fixed_string<default_size>;
using wfs = fixed_wstring<default_size>;
#if FIXSTR_CPP20_CHAR8T_PRESENT
using u8fs = fixed_u8string<default_size>;
#endif // FIXSTR_CPP20_CHAR8T_PRESENT
using u16fs = fixed_u16string<default_size>;
using u32fs = fixed_u32string<default_size>;

namespace utils
{
template <typename T>
constexpr auto equals(T value)
{
    return [v = std::move(value)](const T& a) { return a == v; };
}

template <typename T, std::size_t N>
using array_ref = T (&)[N];

template <std::size_t N>
using literal_ref = array_ref<const char, N>;

template <class InputIt, class OutputIt, class UnaryOperation>
constexpr OutputIt transform(InputIt first1, InputIt last1, OutputIt d_first, UnaryOperation unary_op)
{
    while (first1 != last1)
    {
        *d_first++ = unary_op(*first1++);
    }
    return d_first;
}

template <typename TDestination, std::size_t N>
void convert_literal(literal_ref<N> from, array_ref<TDestination, N> to)
{
    utils::transform(std::begin(from), std::end(from), std::begin(to), [](const auto a) { return static_cast<TDestination>(a); });
}

template <typename TDestination, std::size_t N>
constexpr basic_fixed_string<TDestination, N - 1> to_fs(literal_ref<N> from)
{
    basic_fixed_string<TDestination, N - 1> to;
    utils::transform(std::begin(from), std::end(from), std::begin(to), [](const auto a) { return static_cast<TDestination>(a); });
    return to;
}

template <template <size_t> class T, std::size_t N>
constexpr T<N - 1> to_fs_2(literal_ref<N> from)
{
    T<N - 1> to;
    utils::transform(std::begin(from), std::end(from), std::begin(to), [](const auto a) { return static_cast<typename T<0>::value_type>(a); });
    return to;
}
} // namespace utils

namespace construction
{
namespace default_construction
{
template <typename T>
void check()
{
    T str;
    REQUIRE(std::all_of(str.begin(), str.end(), utils::equals(static_cast<typename T::value_type>(0))));
}
} // namespace default_construction

namespace from_c_array
{
template <typename T>
void check()
{
    using char_t = typename T::value_type;
    constexpr char literal[] = "Hello, world!!!";
    char_t         converted_literal[std::size(literal)]{};
    utils::convert_literal(literal, converted_literal);
    T str(converted_literal);
    REQUIRE(std::equal(str.begin(), str.end(), std::begin(converted_literal)));
}
} // namespace from_c_array

namespace copy
{
template <typename T>
void check()
{
    using char_t = typename T::value_type;
    constexpr char literal[] = "Hello, world!!!";
    char_t         converted_literal[std::size(literal)]{};
    utils::convert_literal(literal, converted_literal);
    T str(converted_literal);
    T str2(str);
    REQUIRE(std::equal(str.begin(), str.end(), str2.begin()));
}

} // namespace copy
} // namespace construction

TEST_CASE("Construction")
{
    using namespace construction;
    SECTION("Default construction makes zero-filled string")
    {
        using namespace default_construction;
        SECTION("fixed_string") { check<fs>(); }
        SECTION("fixed_wstring") { check<wfs>(); }
#if FIXSTR_CPP20_CHAR8T_PRESENT
        SECTION("fixed_u8string") { check<u8fs>(); }
#endif // FIXSTR_CPP20_CHAR8T_PRESENT
        SECTION("fixed_u16string") { check<u16fs>(); }
        SECTION("fixed_u32string") { check<u32fs>(); }
    }

    SECTION("Construction from C-array makes a copy of array")
    {
        using namespace from_c_array;
        SECTION("fixed_string") { check<fs>(); }
        SECTION("fixed_wstring") { check<wfs>(); }
#if FIXSTR_CPP20_CHAR8T_PRESENT
        SECTION("fixed_u8string") { check<u8fs>(); }
#endif // FIXSTR_CPP20_CHAR8T_PRESENT
        SECTION("fixed_u16string") { check<u16fs>(); }
        SECTION("fixed_u32string") { check<u32fs>(); }
    }

    SECTION("Copy construction")
    {
        using namespace copy;
        SECTION("fixed_string") { check<fs>(); }
        SECTION("fixed_wstring") { check<wfs>(); }
#if FIXSTR_CPP20_CHAR8T_PRESENT
        SECTION("fixed_u8string") { check<u8fs>(); }
#endif // FIXSTR_CPP20_CHAR8T_PRESENT
        SECTION("fixed_u16string") { check<u16fs>(); }
        SECTION("fixed_u32string") { check<u32fs>(); }
    }
}

namespace assignment
{
namespace from_c_array
{
template <typename T>
void check()
{
    using char_t = typename T::value_type;
    constexpr char literal[] = "Hello, world!!!";
    char_t         converted_literal[std::size(literal)]{};
    utils::convert_literal(literal, converted_literal);

    T str;
    REQUIRE(!std::equal(str.begin(), str.end(), std::begin(converted_literal)));
    str = converted_literal;
    REQUIRE(std::equal(str.begin(), str.end(), std::begin(converted_literal)));
}
} // namespace from_c_array
namespace from_fixed_string
{
template <typename T>
void check()
{
    using char_t = typename T::value_type;
    constexpr char literal[] = "Hello, world!!!";
    char_t         converted_literal[std::size(literal)]{};
    utils::convert_literal(literal, converted_literal);
    T str2 = converted_literal;

    T str;
    REQUIRE(!std::equal(str.begin(), str.end(), std::begin(converted_literal)));
    str = str2;
    REQUIRE(std::equal(str.begin(), str.end(), std::begin(converted_literal)));
}
} // namespace from_fixed_string
} // namespace assignment

TEST_CASE("Assignment")
{
    using namespace assignment;
    SECTION("Assigment from C array makes a copy")
    {
        using namespace from_c_array;
        SECTION("fixed_string") { check<fs>(); }
        SECTION("fixed_wstring") { check<wfs>(); }
#if FIXSTR_CPP20_CHAR8T_PRESENT
        SECTION("fixed_u8string") { check<u8fs>(); }
#endif // FIXSTR_CPP20_CHAR8T_PRESENT
        SECTION("fixed_u16string") { check<u16fs>(); }
        SECTION("fixed_u32string") { check<u32fs>(); }
    }
    SECTION("Assigment from fixed_string makes a copy")
    {
        using namespace from_fixed_string;
        SECTION("fixed_string") { check<fs>(); }
        SECTION("fixed_wstring") { check<wfs>(); }
#if FIXSTR_CPP20_CHAR8T_PRESENT
        SECTION("fixed_u8string") { check<u8fs>(); }
#endif // FIXSTR_CPP20_CHAR8T_PRESENT
        SECTION("fixed_u16string") { check<u16fs>(); }
        SECTION("fixed_u32string") { check<u32fs>(); }
    }
}

namespace capacity
{
namespace size
{
template <typename T>
void check()
{
    T str;
    REQUIRE(str.size() == default_size);
}
} // namespace size

namespace max_size
{
template <typename T>
void check()
{
    T str;
    REQUIRE(str.max_size() == default_size);
}
} // namespace max_size

namespace length
{
template <typename T>
void check()
{
    T str;
    REQUIRE(str.length() == default_size);
}
} // namespace length

namespace empty
{
template <template <std::size_t> class T>
void check()
{
    T<0> empty;
    T<1> non_empty;
    REQUIRE(empty.empty());
    REQUIRE(!non_empty.empty());
}
} // namespace empty
} // namespace capacity

TEST_CASE("Capacity members")
{
    using namespace capacity;
    SECTION("size")
    {
        using namespace size;
        SECTION("fixed_string") { check<fs>(); }
        SECTION("fixed_wstring") { check<wfs>(); }
#if FIXSTR_CPP20_CHAR8T_PRESENT
        SECTION("fixed_u8string") { check<u8fs>(); }
#endif // FIXSTR_CPP20_CHAR8T_PRESENT
        SECTION("fixed_u16string") { check<u16fs>(); }
        SECTION("fixed_u32string") { check<u32fs>(); }
    }
    SECTION("max_size")
    {
        using namespace max_size;
        SECTION("fixed_string") { check<fs>(); }
        SECTION("fixed_wstring") { check<wfs>(); }
#if FIXSTR_CPP20_CHAR8T_PRESENT
        SECTION("fixed_u8string") { check<u8fs>(); }
#endif // FIXSTR_CPP20_CHAR8T_PRESENT
        SECTION("fixed_u16string") { check<u16fs>(); }
        SECTION("fixed_u32string") { check<u32fs>(); }
    }
    SECTION("length")
    {
        using namespace length;
        SECTION("fixed_string") { check<fs>(); }
        SECTION("fixed_wstring") { check<wfs>(); }
#if FIXSTR_CPP20_CHAR8T_PRESENT
        SECTION("fixed_u8string") { check<u8fs>(); }
#endif // FIXSTR_CPP20_CHAR8T_PRESENT
        SECTION("fixed_u16string") { check<u16fs>(); }
        SECTION("fixed_u32string") { check<u32fs>(); }
    }
    SECTION("empty")
    {
        using namespace empty;
        SECTION("fixed_string") { check<fixed_string>(); }
        SECTION("fixed_wstring") { check<fixed_wstring>(); }
#if FIXSTR_CPP20_CHAR8T_PRESENT
        SECTION("fixed_u8string") { check<fixed_u8string>(); }
#endif // FIXSTR_CPP20_CHAR8T_PRESENT
        SECTION("fixed_u16string") { check<fixed_u16string>(); }
        SECTION("fixed_u32string") { check<fixed_u32string>(); }
    }
}

namespace sv_conversion
{
template <typename T>
void check()
{
    T                            str;
    typename T::string_view_type sv = str;
    REQUIRE(sv.data() == str.data());
    REQUIRE(sv.size() == str.size());
}
} // namespace sv_conversion

TEST_CASE("std::string_view conversion")
{
    using namespace sv_conversion;
    SECTION("fixed_string") { check<fs>(); }
    SECTION("fixed_wstring") { check<wfs>(); }
#if FIXSTR_CPP20_CHAR8T_PRESENT
    SECTION("fixed_u8string") { check<u8fs>(); }
#endif // FIXSTR_CPP20_CHAR8T_PRESENT
    SECTION("fixed_u16string") { check<u16fs>(); }
    SECTION("fixed_u32string") { check<u32fs>(); }
}

namespace utils::traits
{
struct nonesuch
{
    ~nonesuch() = delete;
    nonesuch(nonesuch const&) = delete;
    void operator=(nonesuch const&) = delete;
};
namespace detail
{
template <class Default, class AlwaysVoid, template <class...> class Op, class... Args>
struct detector
{
    using value_t = std::false_type;
    using type = Default;
};

template <class Default, template <class...> class Op, class... Args>
struct detector<Default, std::void_t<Op<Args...>>, Op, Args...>
{
    using value_t = std::true_type;
    using type = Op<Args...>;
};

} // namespace detail

template <template <class...> class Op, class... Args>
using is_detected = typename detail::detector<nonesuch, void, Op, Args...>::value_t;
} // namespace utils::traits

namespace substr
{
namespace pos_equals_size
{
template <typename T>
void check()
{
    using char_t = typename T::value_type;
    constexpr char literal[] = "Hello, world!!!";
    char_t         converted_literal[std::size(literal)]{};
    utils::convert_literal(literal, converted_literal);

    T    str(converted_literal);
    auto substr = str.template substr<str.size()>();
    REQUIRE(substr.empty());
}
} // namespace pos_equals_size

namespace default_parameters
{
template <typename T>
void check()
{
    using char_t = typename T::value_type;
    constexpr char literal[] = "Hello, world!!!";
    char_t         converted_literal[std::size(literal)]{};
    utils::convert_literal(literal, converted_literal);

    T    str(converted_literal);
    auto substr = str.template substr<>();
    REQUIRE(substr == str);
}
} // namespace default_parameters

namespace middle
{
template <typename T>
void check()
{
    using char_t = typename T::value_type;
    constexpr char literal[] = "Hello, world!!!";
    char_t         converted_literal[std::size(literal)]{};
    utils::convert_literal(literal, converted_literal);

    constexpr auto pos = 2;
    constexpr auto count = 3;

    T    str(converted_literal);
    auto fs_substr = str.template substr<pos, count>();
    auto sv_substr = static_cast<typename T::string_view_type>(str).substr(pos, count);
    REQUIRE(fs_substr == sv_substr);
}
} // namespace middle

namespace sfinae
{
template <typename T>
using substr_valid_t = decltype(std::declval<T>().template substr<T{}.size(), T::npos>());

template <typename T>
using substr_invalid_t = decltype(std::declval<T>().template substr<T{}.size() + 1, T::npos>());

template <typename T>
void check()
{
    using namespace utils::traits;

    // for some unknown reason is_detected doesn't work on MSVC version 1920 or older
    // in this particular case even though SFINAE works just fine
#if defined(_MSC_VER) && _MSC_VER > 1920 || !defined(_MSC_VER)
    REQUIRE(is_detected<substr_valid_t, T>::value);
    REQUIRE(!is_detected<substr_invalid_t, T>::value);
    REQUIRE(is_detected<substr_valid_t, const T>::value);
    REQUIRE(!is_detected<substr_invalid_t, const T>::value);
#endif // defined(_MSC_VER) && _MSC_VER > 1920 || !defined(_MSC_VER)
}
} // namespace sfinae
} // namespace substr

TEST_CASE("substr")
{
    using namespace substr;
    SECTION("should be empty if pos equals size")
    {
        using namespace pos_equals_size;
        SECTION("fixed_string") { check<fs>(); }
        SECTION("fixed_wstring") { check<wfs>(); }
#if FIXSTR_CPP20_CHAR8T_PRESENT
        SECTION("fixed_u8string") { check<u8fs>(); }
#endif // FIXSTR_CPP20_CHAR8T_PRESENT
        SECTION("fixed_u16string") { check<u16fs>(); }
        SECTION("fixed_u32string") { check<u32fs>(); }
    }
    SECTION("should copy whole string with default parameters")
    {
        using namespace default_parameters;
        SECTION("fixed_string") { check<fs>(); }
        SECTION("fixed_wstring") { check<wfs>(); }
#if FIXSTR_CPP20_CHAR8T_PRESENT
        SECTION("fixed_u8string") { check<u8fs>(); }
#endif // FIXSTR_CPP20_CHAR8T_PRESENT
        SECTION("fixed_u16string") { check<u16fs>(); }
        SECTION("fixed_u32string") { check<u32fs>(); }
    }
    SECTION("substring in middle")
    {
        using namespace middle;
        SECTION("fixed_string") { check<fs>(); }
        SECTION("fixed_wstring") { check<wfs>(); }
#if FIXSTR_CPP20_CHAR8T_PRESENT
        SECTION("fixed_u8string") { check<u8fs>(); }
#endif // FIXSTR_CPP20_CHAR8T_PRESENT
        SECTION("fixed_u16string") { check<u16fs>(); }
        SECTION("fixed_u32string") { check<u32fs>(); }
    }
    SECTION("sfinae friendliness")
    {
        using namespace sfinae;
        SECTION("fixed_string") { check<fs>(); }
        SECTION("fixed_wstring") { check<wfs>(); }
#if FIXSTR_CPP20_CHAR8T_PRESENT
        SECTION("fixed_u8string") { check<u8fs>(); }
#endif // FIXSTR_CPP20_CHAR8T_PRESENT
        SECTION("fixed_u16string") { check<u16fs>(); }
        SECTION("fixed_u32string") { check<u32fs>(); }
    }
}

namespace plus
{
template <template <std::size_t> class T>
void check()
{
    using char_t = typename T<0>::value_type;
    using sv_t = typename T<0>::string_view_type;

    utils::literal_ref<3> lhs = "Hi";
    utils::literal_ref<6> rhs = "Hello";

    const auto lhs_fs = utils::to_fs<char_t>(lhs);
    const auto rhs_fs = utils::to_fs<char_t>(rhs);
    const auto fs_res = lhs_fs + rhs_fs;

    std::basic_string lhs_std = lhs_fs.data();
    std::basic_string rhs_std = rhs_fs.data();
    const auto        std_res = lhs_std + rhs_std;

    REQUIRE(static_cast<sv_t>(fs_res) == std_res);

    const auto ch = static_cast<char_t>('A');
    REQUIRE(static_cast<sv_t>(lhs_fs + ch) == (lhs_std + ch));
}
} // namespace plus

TEST_CASE("operator+")
{
    using namespace plus;
    SECTION("fixed_string") { check<fixed_string>(); }
    SECTION("fixed_wstring") { check<fixed_wstring>(); }
#if FIXSTR_CPP20_CHAR8T_PRESENT
    SECTION("fixed_u8string") { check<fixed_u8string>(); }
#endif // FIXSTR_CPP20_CHAR8T_PRESENT
    SECTION("fixed_u16string") { check<fixed_u16string>(); }
    SECTION("fixed_u32string") { check<fixed_u32string>(); }
}

#if FIXSTR_CPP20_CNTTP_PRESENT

namespace cnttp
{
template <fixed_string v1, fixed_string v2>
void check()
{
    static_assert(v1 == v2);
}
} // namespace cnttp

TEST_CASE("Class non-type template parameters")
{
    using namespace cnttp;
    check<"Hi", "Hi">();
}

#endif // FIXSTR_CPP20_CNTTP_PRESENT

namespace swapping
{
namespace member
{
template <template <std::size_t> class T>
void check()
{
    using char_t = typename T<0>::value_type;

    utils::literal_ref<6> lhs = "World";
    utils::literal_ref<6> rhs = "Hello";

    auto lhs_fs = utils::to_fs<char_t>(lhs);
    auto rhs_fs = utils::to_fs<char_t>(rhs);

    lhs_fs.swap(rhs_fs);

    REQUIRE(rhs_fs == utils::to_fs<char_t>(lhs));
    REQUIRE(lhs_fs == utils::to_fs<char_t>(rhs));
}
} // namespace member

namespace free
{
template <template <std::size_t> class T>
void check()
{
    using char_t = typename T<0>::value_type;

    utils::literal_ref<6> lhs = "World";
    utils::literal_ref<6> rhs = "Hello";

    auto lhs_fs = utils::to_fs<char_t>(lhs);
    auto rhs_fs = utils::to_fs<char_t>(rhs);

    swap(lhs_fs, rhs_fs);

    REQUIRE(rhs_fs == utils::to_fs<char_t>(lhs));
    REQUIRE(lhs_fs == utils::to_fs<char_t>(rhs));
}
} // namespace free
} // namespace swapping

TEST_CASE("swap")
{
    using namespace swapping;
    SECTION("member")
    {
        using namespace member;
        SECTION("fixed_string") { check<fixed_string>(); }
        SECTION("fixed_wstring") { check<fixed_wstring>(); }
#if FIXSTR_CPP20_CHAR8T_PRESENT
        SECTION("fixed_u8string") { check<fixed_u8string>(); }
#endif // FIXSTR_CPP20_CHAR8T_PRESENT
        SECTION("fixed_u16string") { check<fixed_u16string>(); }
        SECTION("fixed_u32string") { check<fixed_u32string>(); }
    }
    SECTION("free")
    {
        using namespace free;
        SECTION("fixed_string") { check<fixed_string>(); }
        SECTION("fixed_wstring") { check<fixed_wstring>(); }
#if FIXSTR_CPP20_CHAR8T_PRESENT
        SECTION("fixed_u8string") { check<fixed_u8string>(); }
#endif // FIXSTR_CPP20_CHAR8T_PRESENT
        SECTION("fixed_u16string") { check<fixed_u16string>(); }
        SECTION("fixed_u32string") { check<fixed_u32string>(); }
    }
}

namespace hash_support
{
template <template <std::size_t> class T>
void check()
{
    using sv_t = typename T<0>::string_view_type;
    constexpr utils::literal_ref<6> literal = "Hello";
    constexpr auto                  fixed_str = utils::to_fs_2<T>(literal);

    const auto fixed_str_hash = std::hash<T<fixed_str.size()>>()(fixed_str);
    const auto string_view_hash = std::hash<sv_t>()(static_cast<sv_t>(fixed_str));
    REQUIRE(string_view_hash == fixed_str_hash);
}
} // namespace hash_support

TEST_CASE("hash support")
{
    using namespace hash_support;
    SECTION("fixed_string") { check<fixed_string>(); }
    SECTION("fixed_wstring") { check<fixed_wstring>(); }
#if FIXSTR_CPP20_CHAR8T_PRESENT
    SECTION("fixed_u8string") { check<fixed_u8string>(); }
#endif // FIXSTR_CPP20_CHAR8T_PRESENT
    SECTION("fixed_u16string") { check<fixed_u16string>(); }
    SECTION("fixed_u32string") { check<fixed_u32string>(); }
}

namespace front_and_back
{
namespace logic
{
template <template <std::size_t> class T>
void check()
{
    using char_t = typename T<0>::value_type;
    using sv_t = typename T<0>::string_view_type;

    utils::literal_ref<6> literal = "World";

    const auto fs = utils::to_fs<char_t>(literal);

    const auto sv = static_cast<sv_t>(fs);

    REQUIRE(fs.front() == sv.front());
    REQUIRE(fs.back() == sv.back());
}
} // namespace logic

namespace overloads
{
template <typename T>
using front_t = decltype(std::declval<T>().front());
template <typename T>
using back_t = decltype(std::declval<T>().back());

template <template <std::size_t> class T>
void check()
{
    using utils::traits::is_detected;

    REQUIRE(!is_detected<front_t, T<0>>::value);
    REQUIRE(!is_detected<front_t, const T<0>>::value);

    REQUIRE(is_detected<front_t, T<1>>::value);
    REQUIRE(is_detected<front_t, const T<1>>::value);

    REQUIRE(!is_detected<back_t, T<0>>::value);
    REQUIRE(!is_detected<back_t, const T<0>>::value);

    REQUIRE(is_detected<back_t, T<1>>::value);
    REQUIRE(is_detected<back_t, const T<1>>::value);
}
} // namespace overloads
} // namespace front_and_back

TEST_CASE("front and back")
{
    using namespace front_and_back;
    SECTION("main logic")
    {
        using namespace logic;
        SECTION("fixed_string") { check<fixed_string>(); }
        SECTION("fixed_wstring") { check<fixed_wstring>(); }
#if FIXSTR_CPP20_CHAR8T_PRESENT
        SECTION("fixed_u8string") { check<fixed_u8string>(); }
#endif // FIXSTR_CPP20_CHAR8T_PRESENT
        SECTION("fixed_u16string") { check<fixed_u16string>(); }
        SECTION("fixed_u32string") { check<fixed_u32string>(); }
    }
    SECTION("deleted overloads of front and back for size 0")
    {
        using namespace overloads;
        SECTION("fixed_string") { check<fixed_string>(); }
        SECTION("fixed_wstring") { check<fixed_wstring>(); }
#if FIXSTR_CPP20_CHAR8T_PRESENT
        SECTION("fixed_u8string") { check<fixed_u8string>(); }
#endif // FIXSTR_CPP20_CHAR8T_PRESENT
        SECTION("fixed_u16string") { check<fixed_u16string>(); }
        SECTION("fixed_u32string") { check<fixed_u32string>(); }
    }
}

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

#include <algorithm>
#include <minitest/minitest.h>
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
    EXPECT_TRUE(std::all_of(str.begin(), str.end(), utils::equals(static_cast<typename T::value_type>(0))));
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
    EXPECT_TRUE(std::equal(str.begin(), str.end(), std::begin(converted_literal)));
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
    EXPECT_TRUE(std::equal(str.begin(), str.end(), str2.begin()));
}

} // namespace copy
} // namespace construction

TEST(FixedStringTest, DefaultConstructionMakesZeroFilledString) {
    using namespace construction::default_construction;
    check<fs>();
    check<wfs>();
#if FIXSTR_CPP20_CHAR8T_PRESENT
    check<u8fs>();
#endif // FIXSTR_CPP20_CHAR8T_PRESENT
    check<u16fs>();
    check<u32fs>();
}

TEST(FixedStringTest, ConstructionFromCArrayMakesCopyOfArray) {
    using namespace construction::from_c_array;
    check<fs>();
    check<wfs>();
#if FIXSTR_CPP20_CHAR8T_PRESENT
    check<u8fs>();
#endif // FIXSTR_CPP20_CHAR8T_PRESENT
    check<u16fs>();
    check<u32fs>();
}

TEST(FixedStringTest, CopyConstruction) {
    using namespace construction::copy;
    check<fs>();
    check<wfs>();
#if FIXSTR_CPP20_CHAR8T_PRESENT
    check<u8fs>();
#endif // FIXSTR_CPP20_CHAR8T_PRESENT
    check<u16fs>();
    check<u32fs>();
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
    EXPECT_FALSE(std::equal(str.begin(), str.end(), std::begin(converted_literal)));
    str = converted_literal;
    EXPECT_TRUE(std::equal(str.begin(), str.end(), std::begin(converted_literal)));
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
    EXPECT_FALSE(std::equal(str.begin(), str.end(), std::begin(converted_literal)));
    str = str2;
    EXPECT_TRUE(std::equal(str.begin(), str.end(), std::begin(converted_literal)));
}
} // namespace from_fixed_string
} // namespace assignment

TEST(FixedStringTest, AssignmentFromCArrayMakesCopy) {
    using namespace assignment::from_c_array;
    check<fs>();
    check<wfs>();
#if FIXSTR_CPP20_CHAR8T_PRESENT
    check<u8fs>();
#endif // FIXSTR_CPP20_CHAR8T_PRESENT
    check<u16fs>();
    check<u32fs>();
}

TEST(FixedStringTest, AssignmentFromFixedStringMakesCopy) {
    using namespace assignment::from_fixed_string;
    check<fs>();
    check<wfs>();
#if FIXSTR_CPP20_CHAR8T_PRESENT
    check<u8fs>();
#endif // FIXSTR_CPP20_CHAR8T_PRESENT
    check<u16fs>();
    check<u32fs>();
}

namespace capacity
{
namespace size
{
template <typename T>
void check()
{
    T str;
    EXPECT_EQ(str.size(), default_size);
}
} // namespace size

namespace max_size
{
template <typename T>
void check()
{
    T str;
    EXPECT_EQ(str.max_size(), default_size);
}
} // namespace max_size

namespace length
{
template <typename T>
void check()
{
    T str;
    EXPECT_EQ(str.length(), default_size);
}
} // namespace length

namespace empty
{
template <template <std::size_t> class T>
void check()
{
    T<0> empty;
    T<1> non_empty;
    EXPECT_TRUE(empty.empty());
    EXPECT_FALSE(non_empty.empty());
}
} // namespace empty
} // namespace capacity

TEST(FixedStringTest, CapacitySize) {
    using namespace capacity::size;
    check<fs>();
    check<wfs>();
#if FIXSTR_CPP20_CHAR8T_PRESENT
    check<u8fs>();
#endif // FIXSTR_CPP20_CHAR8T_PRESENT
    check<u16fs>();
    check<u32fs>();
}

TEST(FixedStringTest, CapacityMaxSize) {
    using namespace capacity::max_size;
    check<fs>();
    check<wfs>();
#if FIXSTR_CPP20_CHAR8T_PRESENT
    check<u8fs>();
#endif // FIXSTR_CPP20_CHAR8T_PRESENT
    check<u16fs>();
    check<u32fs>();
}

TEST(FixedStringTest, CapacityLength) {
    using namespace capacity::length;
    check<fs>();
    check<wfs>();
#if FIXSTR_CPP20_CHAR8T_PRESENT
    check<u8fs>();
#endif // FIXSTR_CPP20_CHAR8T_PRESENT
    check<u16fs>();
    check<u32fs>();
}

TEST(FixedStringTest, CapacityEmpty) {
    using namespace capacity::empty;
    check<fixed_string>();
    check<fixed_wstring>();
#if FIXSTR_CPP20_CHAR8T_PRESENT
    check<fixed_u8string>();
#endif // FIXSTR_CPP20_CHAR8T_PRESENT
    check<fixed_u16string>();
    check<fixed_u32string>();
}

namespace sv_conversion
{
template <typename T>
void check()
{
    T                            str;
    typename T::string_view_type sv = str;
    EXPECT_EQ(sv.data(), str.data());
    EXPECT_EQ(sv.size(), str.size());
}
} // namespace sv_conversion

TEST(FixedStringTest, StdStringViewConversion) {
    using namespace sv_conversion;
    check<fs>();
    check<wfs>();
#if FIXSTR_CPP20_CHAR8T_PRESENT
    check<u8fs>();
#endif // FIXSTR_CPP20_CHAR8T_PRESENT
    check<u16fs>();
    check<u32fs>();
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
    EXPECT_TRUE(substr.empty());
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
    EXPECT_EQ(substr, str);
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
    EXPECT_EQ(fs_substr, sv_substr);
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
    EXPECT_TRUE((is_detected<substr_valid_t, T>::value));
    EXPECT_FALSE((is_detected<substr_invalid_t, T>::value));
    EXPECT_TRUE((is_detected<substr_valid_t, const T>::value));
    EXPECT_FALSE((is_detected<substr_invalid_t, const T>::value));
#endif // defined(_MSC_VER) && _MSC_VER > 1920 || !defined(_MSC_VER)
}
} // namespace sfinae
} // namespace substr

TEST(FixedStringTest, SubstrPosEqualsSize) {
    using namespace substr::pos_equals_size;
    check<fs>();
    check<wfs>();
#if FIXSTR_CPP20_CHAR8T_PRESENT
    check<u8fs>();
#endif // FIXSTR_CPP20_CHAR8T_PRESENT
    check<u16fs>();
    check<u32fs>();
}

TEST(FixedStringTest, SubstrDefaultParameters) {
    using namespace substr::default_parameters;
    check<fs>();
    check<wfs>();
#if FIXSTR_CPP20_CHAR8T_PRESENT
    check<u8fs>();
#endif // FIXSTR_CPP20_CHAR8T_PRESENT
    check<u16fs>();
    check<u32fs>();
}

TEST(FixedStringTest, SubstrMiddle) {
    using namespace substr::middle;
    check<fs>();
    check<wfs>();
#if FIXSTR_CPP20_CHAR8T_PRESENT
    check<u8fs>();
#endif // FIXSTR_CPP20_CHAR8T_PRESENT
    check<u16fs>();
    check<u32fs>();
}

TEST(FixedStringTest, SubstrSfinaeFriendliness) {
    using namespace substr::sfinae;
    check<fs>();
    check<wfs>();
#if FIXSTR_CPP20_CHAR8T_PRESENT
    check<u8fs>();
#endif // FIXSTR_CPP20_CHAR8T_PRESENT
    check<u16fs>();
    check<u32fs>();
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

    EXPECT_EQ(static_cast<sv_t>(fs_res), std_res);

    const auto ch = static_cast<char_t>('A');
    EXPECT_EQ(static_cast<sv_t>(lhs_fs + ch), (lhs_std + ch));
}
} // namespace plus

TEST(FixedStringTest, OperatorPlus) {
    using namespace plus;
    check<fixed_string>();
    check<fixed_wstring>();
#if FIXSTR_CPP20_CHAR8T_PRESENT
    check<fixed_u8string>();
#endif // FIXSTR_CPP20_CHAR8T_PRESENT
    check<fixed_u16string>();
    check<fixed_u32string>();
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

TEST(FixedStringTest, ClassNonTypeTemplateParameters) {
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

    EXPECT_EQ(rhs_fs, utils::to_fs<char_t>(lhs));
    EXPECT_EQ(lhs_fs, utils::to_fs<char_t>(rhs));
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

    EXPECT_EQ(rhs_fs, utils::to_fs<char_t>(lhs));
    EXPECT_EQ(lhs_fs, utils::to_fs<char_t>(rhs));
}
} // namespace free
} // namespace swapping

TEST(FixedStringTest, SwapMember) {
    using namespace swapping::member;
    check<fixed_string>();
    check<fixed_wstring>();
#if FIXSTR_CPP20_CHAR8T_PRESENT
    check<fixed_u8string>();
#endif // FIXSTR_CPP20_CHAR8T_PRESENT
    check<fixed_u16string>();
    check<fixed_u32string>();
}

TEST(FixedStringTest, SwapFree) {
    using namespace swapping::free;
    check<fixed_string>();
    check<fixed_wstring>();
#if FIXSTR_CPP20_CHAR8T_PRESENT
    check<fixed_u8string>();
#endif // FIXSTR_CPP20_CHAR8T_PRESENT
    check<fixed_u16string>();
    check<fixed_u32string>();
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
    EXPECT_EQ(string_view_hash, fixed_str_hash);
}
} // namespace hash_support

TEST(FixedStringTest, HashSupport) {
    using namespace hash_support;
    check<fixed_string>();
    check<fixed_wstring>();
#if FIXSTR_CPP20_CHAR8T_PRESENT
    check<fixed_u8string>();
#endif // FIXSTR_CPP20_CHAR8T_PRESENT
    check<fixed_u16string>();
    check<fixed_u32string>();
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

    EXPECT_EQ(fs.front(), sv.front());
    EXPECT_EQ(fs.back(), sv.back());
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

    EXPECT_FALSE((is_detected<front_t, T<0>>::value));
    EXPECT_FALSE((is_detected<front_t, const T<0>>::value));

    EXPECT_TRUE((is_detected<front_t, T<1>>::value));
    EXPECT_TRUE((is_detected<front_t, const T<1>>::value));

    EXPECT_FALSE((is_detected<back_t, T<0>>::value));
    EXPECT_FALSE((is_detected<back_t, const T<0>>::value));

    EXPECT_TRUE((is_detected<back_t, T<1>>::value));
    EXPECT_TRUE((is_detected<back_t, const T<1>>::value));
}
} // namespace overloads
} // namespace front_and_back

TEST(FixedStringTest, FrontAndBackMainLogic) {
    using namespace front_and_back::logic;
    check<fixed_string>();
    check<fixed_wstring>();
#if FIXSTR_CPP20_CHAR8T_PRESENT
    check<fixed_u8string>();
#endif // FIXSTR_CPP20_CHAR8T_PRESENT
    check<fixed_u16string>();
    check<fixed_u32string>();
}

TEST(FixedStringTest, FrontAndBackDeletedOverloads) {
    using namespace front_and_back::overloads;
    check<fixed_string>();
    check<fixed_wstring>();
#if FIXSTR_CPP20_CHAR8T_PRESENT
    check<fixed_u8string>();
#endif // FIXSTR_CPP20_CHAR8T_PRESENT
    check<fixed_u16string>();
    check<fixed_u32string>();
}

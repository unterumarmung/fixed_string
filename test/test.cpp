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

template <typename TDestination, std::size_t N>
void convert_literal(literal_ref<N> from, array_ref<TDestination, N> to)
{
    std::transform(std::begin(from), std::end(from), std::begin(to), [](const auto a) { return static_cast<TDestination>(a); });
}

template <typename TDestination, std::size_t N>
basic_fixed_string<TDestination, N - 1> to_fs(literal_ref<N> from)
{
    basic_fixed_string<TDestination, N - 1> to;
    std::transform(std::begin(from), std::end(from), std::begin(to), [](const auto a) { return static_cast<TDestination>(a); });
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

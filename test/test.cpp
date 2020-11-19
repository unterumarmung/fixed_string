#define CATCH_CONFIG_MAIN

#include <catch.hpp>
#include <algorithm>

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
template<typename T>
constexpr auto equals(T value)
{
    return [v = std::move(value)](const T &a)
    {
        return a == v;
    };
}

template <typename T, std::size_t N>
using array_ref = T(&)[N];

template <std::size_t N>
using literal_ref = array_ref<const char, N>;

template <typename TDestination, std::size_t N>
void convert_literal(literal_ref<N> from, array_ref<TDestination, N> to)
{
    std::transform(std::begin(from), std::end(from), std::begin(to),
                   [](const auto a) { return static_cast<TDestination>(a); }
    );
}

}

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
    char_t converted_literal[std::size(literal)]{};
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
    char_t converted_literal[std::size(literal)]{};
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

namespace assignment {
namespace from_c_array {
template <typename T>
void check()
{
    using char_t = typename T::value_type;
    constexpr char literal[] = "Hello, world!!!";
    char_t converted_literal[std::size(literal)]{};
    utils::convert_literal(literal, converted_literal);


    T str;
    REQUIRE(!std::equal(str.begin(), str.end(), std::begin(converted_literal)));
    str = converted_literal;
    REQUIRE(std::equal(str.begin(), str.end(), std::begin(converted_literal)));
}
} // namespace from_c_array
namespace from_fixed_string {
template <typename T>
void check()
{
    using char_t = typename T::value_type;
    constexpr char literal[] = "Hello, world!!!";
    char_t converted_literal[std::size(literal)]{};
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

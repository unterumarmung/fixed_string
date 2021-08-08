#include <fixed_string.hpp>
#include <iterator>

int main()
{
    fixstr::fixed_string s = "Exemplar";
    static_assert(s.size() == 8);
    static_assert(s.size() == s.length());
    constexpr auto iterator_distance = static_cast<decltype(s)::size_type>(std::distance(s.begin(), s.end()));
    static_assert(s.size() == iterator_distance);

    fixstr::fixed_u32string a = U"ハロー・ワールド"; // 8 code points
    static_assert(a.size() == 8);                    // 8 code units in UTF-32

    fixstr::fixed_u16string b = u"ハロー・ワールド"; // 8 code points
    static_assert(b.size() == 8);                    // 8 code units in UTF-16

    fixstr::fixed_string c = "ハロー・ワールド"; // 8 code points
    static_assert(c.size() == 24);               // 24 code units in UTF-8

#if __cplusplus >= 202002
    fixstr::fixed_u8string d = u8"ハロー・ワールド"; // 8 code points
    static_assert(d.size() == 24);                   // 24 code units in UTF-8
#endif
}
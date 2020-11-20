#ifndef FIXED_STRING_HPP
#define FIXED_STRING_HPP

#include <array>
#include <iterator>
#include <string_view>

#define FIXSTR_CPP20_CHAR8T_PRESENT __cpp_char8_t

namespace fixstr
{

template <typename TChar, std::size_t N, typename TTraits = std::char_traits<TChar>>
struct basic_fixed_string
{
    // exposition only
    using storage_type = std::array<TChar, N + 1>;
    storage_type _data{};

    using traits_type = TTraits;
    using value_type = TChar;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using reference = value_type&;
    using const_reference = const value_type&;
    using iterator = typename storage_type::iterator;
    using const_iterator = typename storage_type::const_iterator;
    using reverse_iterator = typename storage_type::reverse_iterator;
    using const_reverse_iterator = typename storage_type::const_reverse_iterator;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using string_view_type = std::basic_string_view<value_type, traits_type>;
    static constexpr auto npos = string_view_type::npos;

    constexpr basic_fixed_string() noexcept { _data.fill(static_cast<value_type>(0)); }

    constexpr basic_fixed_string(const value_type (&array)[N + 1]) noexcept // NOLINT(google-explicit-constructor)
    {
        std::copy(std::begin(array), std::end(array), _data.begin());
    }

    constexpr basic_fixed_string(basic_fixed_string const& other) noexcept { std::copy(other.begin(), other.end(), _data.begin()); }

    constexpr basic_fixed_string& operator=(const basic_fixed_string& other) noexcept
    {
        std::copy(other.begin(), other.end(), _data.begin());
        return *this;
    }

    constexpr basic_fixed_string& operator=(const value_type (&array)[N + 1]) noexcept
    {
        std::copy(std::begin(array), std::end(array), _data.begin());
        return *this;
    }

    // iterators
    [[nodiscard]] constexpr iterator       begin() noexcept { return _data.begin(); }
    [[nodiscard]] constexpr const_iterator begin() const noexcept { return _data.begin(); }
    [[nodiscard]] constexpr iterator       end() noexcept { return _data.end(); }
    [[nodiscard]] constexpr const_iterator end() const noexcept { return _data.end(); }
    [[nodiscard]] constexpr const_iterator cbegin() const noexcept { return _data.cbegin(); }
    [[nodiscard]] constexpr const_iterator cend() const noexcept { return _data.cend(); }
    [[nodiscard]] constexpr iterator       rbegin() noexcept { return _data.rbegin(); }
    [[nodiscard]] constexpr const_iterator rbegin() const noexcept { return _data.rbegin(); }
    [[nodiscard]] constexpr iterator       rend() noexcept { return _data.rend(); }
    [[nodiscard]] constexpr const_iterator rend() const noexcept { return _data.rend(); }
    [[nodiscard]] constexpr const_iterator crbegin() const noexcept { return _data.crbegin(); }
    [[nodiscard]] constexpr const_iterator crend() const noexcept { return _data.crend(); }

    // capacity
    [[nodiscard]] constexpr size_type size() const noexcept { return N; }
    [[nodiscard]] constexpr size_type length() const noexcept { return N; }
    [[nodiscard]] constexpr size_type max_size() const noexcept { return N; }
    [[nodiscard]] constexpr bool      empty() const noexcept { return N == 0; }

    // element access
    [[nodiscard]] constexpr reference       operator[](size_type n) { return _data[n]; }
    [[nodiscard]] constexpr const_reference operator[](size_type n) const { return _data[n]; }
    [[nodiscard]] constexpr reference       at(size_type n) { return _data.at(n); }
    [[nodiscard]] constexpr const_reference at(size_type n) const { return _data.at(n); }
    [[nodiscard]] constexpr reference       front() { return _data.front(); }
    [[nodiscard]] constexpr const_reference front() const { return _data.front(); }
    [[nodiscard]] constexpr reference       back() { return _data.back(); }
    [[nodiscard]] constexpr const_reference back() const { return _data.back(); }

  private:
    template <size_t M>
    using same_with_other_size = basic_fixed_string<value_type, M, traits_type>;

  public:
    // string operations
    [[nodiscard]] constexpr pointer       data() noexcept { return _data.data(); }
    [[nodiscard]] constexpr const_pointer data() const noexcept { return _data.data(); }
    [[nodiscard]] constexpr               operator string_view_type() const noexcept // NOLINT(google-explicit-constructor)
    {
        return {data(), N};
    }

    template <size_t M>
    [[nodiscard]] constexpr size_type find(const same_with_other_size<M>& str, size_type pos = 0) const noexcept
    {
        if constexpr (M > N)
            return npos;
        return sv().find(str.sv(), pos);
    }
    [[nodiscard]] constexpr size_type find(string_view_type sv, size_type pos = 0) const noexcept { return sv().find(sv, pos); }
    [[nodiscard]] constexpr size_type find(const value_type* s, size_type pos, size_type n) const { return sv().find(s, pos, n); }
    [[nodiscard]] constexpr size_type find(const value_type* s, size_type pos = 0) const { return sv().find(s, pos); }
    [[nodiscard]] constexpr size_type find(value_type c, size_type pos = 0) const noexcept { return sv().find(c, pos); }

    template <size_t M>
    [[nodiscard]] constexpr size_type rfind(const same_with_other_size<M>& str, size_type pos = npos) const noexcept
    {
        if constexpr (M > N)
            return npos;
        return sv().rfind(str.sv(), pos);
    }
    [[nodiscard]] constexpr size_type rfind(string_view_type sv, size_type pos = npos) const noexcept { return sv().rfind(sv, pos); }
    [[nodiscard]] constexpr size_type rfind(const value_type* s, size_type pos, size_type n) const { return sv().rfind(s, pos, n); }
    [[nodiscard]] constexpr size_type rfind(const value_type* s, size_type pos = npos) const { return sv().rfind(s, pos); }
    [[nodiscard]] constexpr size_type rfind(value_type c, size_type pos = npos) const noexcept { return sv().rfind(c, pos); }

    template <size_t M>
    [[nodiscard]] constexpr size_type find_first_of(const same_with_other_size<M>& str, size_type pos = 0) const noexcept
    {
        if constexpr (M > N)
            return npos;
        return sv().find_first_of(str.sv(), pos);
    }
    [[nodiscard]] constexpr size_type find_first_of(string_view_type sv, size_type pos = 0) const noexcept { return sv().find_first_of(sv, pos); }
    [[nodiscard]] constexpr size_type find_first_of(const value_type* s, size_type pos, size_type n) const { return sv().find_first_of(s, pos, n); }
    [[nodiscard]] constexpr size_type find_first_of(const value_type* s, size_type pos = 0) const { return sv().find_first_of(s, pos); }
    [[nodiscard]] constexpr size_type find_first_of(value_type c, size_type pos = 0) const noexcept { return sv().find_first_of(c, pos); }

    template <size_t M>
    [[nodiscard]] constexpr size_type find_last_of(const same_with_other_size<M>& str, size_type pos = npos) const noexcept
    {
        if constexpr (M > N)
            return npos;
        return sv().find_last_of(str.sv(), pos);
    }
    [[nodiscard]] constexpr size_type find_last_of(string_view_type sv, size_type pos = npos) const noexcept { return sv().find_last_of(sv, pos); }
    [[nodiscard]] constexpr size_type find_last_of(const value_type* s, size_type pos, size_type n) const { return sv().find_last_of(s, pos, n); }
    [[nodiscard]] constexpr size_type find_last_of(const value_type* s, size_type pos = npos) const { return sv().find_last_of(s, pos); }
    [[nodiscard]] constexpr size_type find_last_of(value_type c, size_type pos = npos) const noexcept { return sv().find_last_of(c, pos); }

    template <size_t M>
    [[nodiscard]] constexpr size_type find_first_not_of(const same_with_other_size<M>& str, size_type pos = 0) const noexcept
    {
        if constexpr (M > N)
            return npos;
        return sv().find_first_of(str.sv(), pos);
    }
    [[nodiscard]] constexpr size_type find_first_not_of(string_view_type sv, size_type pos = 0) const noexcept { return sv().find_first_not_of(sv, pos); }
    [[nodiscard]] constexpr size_type find_first_not_of(const value_type* s, size_type pos, size_type n) const { return sv().find_first_not_of(s, pos, n); }
    [[nodiscard]] constexpr size_type find_first_not_of(const value_type* s, size_type pos = 0) const { return sv().find_first_not_of(s, pos); }
    [[nodiscard]] constexpr size_type find_first_not_of(value_type c, size_type pos = 0) const noexcept { return sv().find_first_not_of(c, pos); }

    template <size_t M>
    [[nodiscard]] constexpr size_type find_last_not_of(const same_with_other_size<M>& str, size_type pos = npos) const noexcept
    {
        if constexpr (M > N)
            return npos;
        return sv().find_last_of(str.sv(), pos);
    }
    [[nodiscard]] constexpr size_type find_last_not_of(string_view_type sv, size_type pos = npos) const noexcept { return sv().find_last_not_of(sv, pos); }
    [[nodiscard]] constexpr size_type find_last_not_of(const value_type* s, size_type pos, size_type n) const { return sv().find_last_not_of(s, pos, n); }
    [[nodiscard]] constexpr size_type find_last_not_of(const value_type* s, size_type pos = npos) const { return sv().find_last_not_of(s, pos); }
    [[nodiscard]] constexpr size_type find_last_not_of(value_type c, size_type pos = npos) const noexcept { return sv().find_last_not_of(c, pos); }

  private:
    constexpr string_view_type sv() { return *this; }
};

template <size_t N>
using fixed_string = basic_fixed_string<char, N>;

#if FIXSTR_CPP20_CHAR8T_PRESENT
template <size_t N>
using fixed_u8string = basic_fixed_string<char8_t, N>;
#endif // FIXSTR_CPP20_CHAR8T_PRESENT

template <size_t N>
using fixed_u16string = basic_fixed_string<char16_t, N>;

template <size_t N>
using fixed_u32string = basic_fixed_string<char32_t, N>;

template <size_t N>
using fixed_wstring = basic_fixed_string<wchar_t, N>;

} // namespace fixstr

#endif // FIXED_STRING_HPP

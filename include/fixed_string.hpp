#ifndef FIXED_STRING_HPP
#define FIXED_STRING_HPP

#include <array>
#include <iterator>
#include <string_view>

#define FIXSTR_CPP20_CHAR8T_PRESENT __cpp_char8_t
#define FIXSTR_CPP20_SPACESHIP_OPERATOR_PRESENT __cpp_lib_three_way_comparison

#define FIXSTR_CPP20_CONSTEXPR_ALGORITHMS_PRESENT (__cpp_lib_constexpr_algorithms)

namespace fixstr
{

namespace details
{
template <typename InputIterator, typename OutputIterator>
constexpr OutputIterator copy(InputIterator first, InputIterator last, OutputIterator d_first)
{
#if FIXSTR_CPP20_CONSTEXPR_ALGORITHMS_PRESENT
    return std::copy(first, last, d_first);
#else
    while (first != last)
    {
        *d_first++ = *first++;
    }
    return d_first;
#endif // FIXSTR_CPP20_CONSTEXPR_ALGORITHMS_PRESENT
}

template <typename ForwardIterator, typename T>
constexpr void fill(ForwardIterator first, ForwardIterator last, const T& value)
{
#if FIXSTR_CPP20_CONSTEXPR_ALGORITHMS_PRESENT
    std::fill(first, last, value);
#else
    for (; first != last; ++first)
    {
        *first = value;
    }
#endif // FIXSTR_CPP20_CONSTEXPR_ALGORITHMS_PRESENT
}

} // namespace details

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

    constexpr basic_fixed_string() noexcept { details::fill(_data.begin(), _data.end(), static_cast<value_type>(0)); }

    constexpr basic_fixed_string(const value_type (&array)[N + 1]) noexcept // NOLINT(google-explicit-constructor)
    {
        details::copy(std::begin(array), std::end(array), _data.begin());
    }

    constexpr basic_fixed_string(basic_fixed_string const& other) noexcept { details::copy(other.begin(), other.end(), _data.begin()); }

    constexpr basic_fixed_string& operator=(const basic_fixed_string& other) noexcept
    {
        details::copy(other.begin(), other.end(), _data.begin());
        return *this;
    }

    constexpr basic_fixed_string& operator=(const value_type (&array)[N + 1]) noexcept
    {
        details::copy(std::begin(array), std::end(array), _data.begin());
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

    template <size_type pos, size_type count, size_type size>
    constexpr static size_type calculate_substr_size()
    {
        if constexpr (pos >= size)
            return 0;

        constexpr size_type rcount = std::min(count, size - pos);

        return rcount;
    }

    template <size_type pos, size_type count>
    using substr_result_type = same_with_other_size<calculate_substr_size<pos, count, N>()>;

  public:
    // string operations
    [[nodiscard]] constexpr pointer       data() noexcept { return _data.data(); }
    [[nodiscard]] constexpr const_pointer data() const noexcept { return _data.data(); }
    [[nodiscard]] constexpr               operator string_view_type() const noexcept // NOLINT(google-explicit-constructor)
    {
        return {data(), N};
    }

    template <size_type pos = 0, size_type count = npos>
    [[nodiscard]] constexpr substr_result_type<pos, count> substr()
    {
        static_assert(pos <= N, "pos cannot be larger than size!");
        constexpr size_type rcount = calculate_substr_size<pos, count, N>();
        substr_result_type<pos, count> result;
        details::copy(begin() + pos, begin() + pos + rcount, result.begin());
        return result;
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

    [[nodiscard]] constexpr int compare(string_view_type v) const noexcept { return sv().compare(v); }
    [[nodiscard]] constexpr int compare(size_type pos1, size_type count1, string_view_type v) const { return sv().compare(pos1, count1, v); }
    [[nodiscard]] constexpr int compare(size_type pos1, size_type count1, string_view_type v, size_type pos2, size_type count2) const
    {
        return sv().compare(pos1, count1, v, pos2, count2);
    }
    [[nodiscard]] constexpr int compare(const value_type* s) const { return sv().compare(s); }
    [[nodiscard]] constexpr int compare(size_type pos1, size_type count1, const value_type* s) const { return sv().compare(pos1, count1, s); }
    [[nodiscard]] constexpr int compare(size_type pos1, size_type count1, const value_type* s, size_type count2) const
    {
        return sv().compare(pos1, count1, s, count2);
    }

    [[nodiscard]] constexpr bool starts_with(string_view_type v) const noexcept { return sv().substr(0, v.size()) == v; }
    [[nodiscard]] constexpr bool starts_with(char c) const noexcept { return !empty() && traits_type::eq(front(), c); }
    [[nodiscard]] constexpr bool starts_with(const value_type* s) const noexcept { return starts_with(string_view_type(s)); }

    [[nodiscard]] constexpr bool ends_with(string_view_type sv) const noexcept { return size() >= sv.size() && compare(size() - sv.size(), npos, sv) == 0; }
    [[nodiscard]] constexpr bool ends_with(value_type c) const noexcept { return !empty() && traits_type::eq(back(), c); }
    [[nodiscard]] constexpr bool ends_with(const value_type* s) const { return ends_with(string_view_type(s)); }

    [[nodiscard]] constexpr bool contains(string_view_type sv) const noexcept { return find(sv) != npos; }
    [[nodiscard]] constexpr bool contains(value_type c) const noexcept { return find(c) != npos; }
    [[nodiscard]] constexpr bool contains(const value_type* s) const { return find(s) != npos; }

  private:
    constexpr string_view_type sv() { return *this; }
};

template <typename TChar, typename TTraits, size_t M1, size_t M2>
[[nodiscard]] constexpr bool operator==(const basic_fixed_string<TChar, M1, TTraits>& lhs, const basic_fixed_string<TChar, M2, TTraits>& rhs)
{
    if constexpr (M1 != M2)
        return false;
    using lhs_type = std::decay_t<decltype(lhs)>;
    using sv_type = typename lhs_type::string_view_type;
    return static_cast<sv_type>(lhs) == rhs;
}

template <typename TChar, typename TTraits, size_t N>
[[nodiscard]] constexpr bool operator==(const basic_fixed_string<TChar, N, TTraits>& lhs, std::basic_string_view<TChar, TTraits> rhs)
{
    using lhs_type = std::decay_t<decltype(lhs)>;
    using sv_type = typename lhs_type::string_view_type;
    return static_cast<sv_type>(lhs) == rhs;
}

template <typename TChar, typename TTraits, size_t N>
[[nodiscard]] constexpr bool operator==(std::basic_string_view<TChar, TTraits> lhs, const basic_fixed_string<TChar, N, TTraits>& rhs)
{
    using rhs_type = std::decay_t<decltype(rhs)>;
    using sv_type = typename rhs_type::string_view_type;
    return lhs == static_cast<sv_type>(rhs);
}

#if FIXSTR_CPP20_SPACESHIP_OPERATOR_PRESENT

template <typename TChar, typename TTraits, size_t M1, size_t M2>
[[nodiscard]] constexpr auto operator<=>(const basic_fixed_string<TChar, M1, TTraits>& lhs, const basic_fixed_string<TChar, M2, TTraits>& rhs)
{
    using lhs_type = std::decay_t<decltype(lhs)>;
    using sv_type = typename lhs_type::string_view_type;
    return static_cast<sv_type>(lhs) <=> rhs;
}

template <typename TChar, typename TTraits, size_t N>
[[nodiscard]] constexpr auto operator<=>(const basic_fixed_string<TChar, N, TTraits>& lhs, std::basic_string_view<TChar, TTraits> rhs)
{
    using lhs_type = std::decay_t<decltype(lhs)>;
    using sv_type = typename lhs_type::string_view_type;
    return static_cast<sv_type>(lhs) <=> rhs;
}

template <typename TChar, typename TTraits, size_t N>
[[nodiscard]] constexpr auto operator<=>(std::basic_string_view<TChar, TTraits> lhs, const basic_fixed_string<TChar, N, TTraits>& rhs)
{
    using rhs_type = std::decay_t<decltype(rhs)>;
    using sv_type = typename rhs_type::string_view_type;
    return lhs <=> static_cast<sv_type>(rhs);
}

#else

template <typename TChar, typename TTraits, size_t M1, size_t M2>
[[nodiscard]] constexpr bool operator!=(const basic_fixed_string<TChar, M1, TTraits>& lhs, const basic_fixed_string<TChar, M2, TTraits>& rhs)
{
    if constexpr (M1 != M2)
        return true;
    using lhs_type = std::decay_t<decltype(lhs)>;
    using sv_type = typename lhs_type::string_view_type;
    return static_cast<sv_type>(lhs) != rhs;
}

template <typename TChar, typename TTraits, size_t N>
[[nodiscard]] constexpr bool operator!=(const basic_fixed_string<TChar, N, TTraits>& lhs, std::basic_string_view<TChar, TTraits> rhs)
{
    using lhs_type = std::decay_t<decltype(lhs)>;
    using sv_type = typename lhs_type::string_view_type;
    return static_cast<sv_type>(lhs) != rhs;
}

template <typename TChar, typename TTraits, size_t N>
[[nodiscard]] constexpr bool operator!=(std::basic_string_view<TChar, TTraits> lhs, const basic_fixed_string<TChar, N, TTraits>& rhs)
{
    using rhs_type = std::decay_t<decltype(rhs)>;
    using sv_type = typename rhs_type::string_view_type;
    return lhs != static_cast<sv_type>(rhs);
}

template <typename TChar, typename TTraits, size_t M1, size_t M2>
[[nodiscard]] constexpr bool operator<(const basic_fixed_string<TChar, M1, TTraits>& lhs, const basic_fixed_string<TChar, M2, TTraits>& rhs)
{
    using lhs_type = std::decay_t<decltype(lhs)>;
    using sv_type = typename lhs_type::string_view_type;
    return static_cast<sv_type>(lhs) < rhs;
}

template <typename TChar, typename TTraits, size_t N>
[[nodiscard]] constexpr bool operator<(const basic_fixed_string<TChar, N, TTraits>& lhs, std::basic_string_view<TChar, TTraits> rhs)
{
    using lhs_type = std::decay_t<decltype(lhs)>;
    using sv_type = typename lhs_type::string_view_type;
    return static_cast<sv_type>(lhs) < rhs;
}

template <typename TChar, typename TTraits, size_t N>
[[nodiscard]] constexpr bool operator<(std::basic_string_view<TChar, TTraits> lhs, const basic_fixed_string<TChar, N, TTraits>& rhs)
{
    using rhs_type = std::decay_t<decltype(rhs)>;
    using sv_type = typename rhs_type::string_view_type;
    return lhs < static_cast<sv_type>(rhs);
}

template <typename TChar, typename TTraits, size_t M1, size_t M2>
[[nodiscard]] constexpr bool operator<=(const basic_fixed_string<TChar, M1, TTraits>& lhs, const basic_fixed_string<TChar, M2, TTraits>& rhs)
{
    using lhs_type = std::decay_t<decltype(lhs)>;
    using sv_type = typename lhs_type::string_view_type;
    return static_cast<sv_type>(lhs) <= rhs;
}

template <typename TChar, typename TTraits, size_t N>
[[nodiscard]] constexpr bool operator<=(const basic_fixed_string<TChar, N, TTraits>& lhs, std::basic_string_view<TChar, TTraits> rhs)
{
    using lhs_type = std::decay_t<decltype(lhs)>;
    using sv_type = typename lhs_type::string_view_type;
    return static_cast<sv_type>(lhs) <= rhs;
}

template <typename TChar, typename TTraits, size_t N>
[[nodiscard]] constexpr bool operator<=(std::basic_string_view<TChar, TTraits> lhs, const basic_fixed_string<TChar, N, TTraits>& rhs)
{
    using rhs_type = std::decay_t<decltype(rhs)>;
    using sv_type = typename rhs_type::string_view_type;
    return lhs <= static_cast<sv_type>(rhs);
}

template <typename TChar, typename TTraits, size_t M1, size_t M2>
[[nodiscard]] constexpr bool operator>(const basic_fixed_string<TChar, M1, TTraits>& lhs, const basic_fixed_string<TChar, M2, TTraits>& rhs)
{
    using lhs_type = std::decay_t<decltype(lhs)>;
    using sv_type = typename lhs_type::string_view_type;
    return static_cast<sv_type>(lhs) > rhs;
}

template <typename TChar, typename TTraits, size_t N>
[[nodiscard]] constexpr bool operator>(const basic_fixed_string<TChar, N, TTraits>& lhs, std::basic_string_view<TChar, TTraits> rhs)
{
    using lhs_type = std::decay_t<decltype(lhs)>;
    using sv_type = typename lhs_type::string_view_type;
    return static_cast<sv_type>(lhs) > rhs;
}

template <typename TChar, typename TTraits, size_t N>
[[nodiscard]] constexpr bool operator>(std::basic_string_view<TChar, TTraits> lhs, const basic_fixed_string<TChar, N, TTraits>& rhs)
{
    using rhs_type = std::decay_t<decltype(rhs)>;
    using sv_type = typename rhs_type::string_view_type;
    return lhs > static_cast<sv_type>(rhs);
}

template <typename TChar, typename TTraits, size_t M1, size_t M2>
[[nodiscard]] constexpr bool operator>=(const basic_fixed_string<TChar, M1, TTraits>& lhs, const basic_fixed_string<TChar, M2, TTraits>& rhs)
{
    using lhs_type = std::decay_t<decltype(lhs)>;
    using sv_type = typename lhs_type::string_view_type;
    return static_cast<sv_type>(lhs) >= rhs;
}

template <typename TChar, typename TTraits, size_t N>
[[nodiscard]] constexpr bool operator>=(const basic_fixed_string<TChar, N, TTraits>& lhs, std::basic_string_view<TChar, TTraits> rhs)
{
    using lhs_type = std::decay_t<decltype(lhs)>;
    using sv_type = typename lhs_type::string_view_type;
    return static_cast<sv_type>(lhs) >= rhs;
}

template <typename TChar, typename TTraits, size_t N>
[[nodiscard]] constexpr bool operator>=(std::basic_string_view<TChar, TTraits> lhs, const basic_fixed_string<TChar, N, TTraits>& rhs)
{
    using rhs_type = std::decay_t<decltype(rhs)>;
    using sv_type = typename rhs_type::string_view_type;
    return lhs >= static_cast<sv_type>(rhs);
}

#endif // FIXSTR_CPP20_SPACESHIP_OPERATOR_PRESENT

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

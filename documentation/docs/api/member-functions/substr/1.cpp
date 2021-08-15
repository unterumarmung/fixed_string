template <size_type pos = 0, size_type count = npos>
    requires pos <= N
[[nodiscard]] constexpr basic_fixed_string<TChar, M, TTraits> substr() const noexcept;
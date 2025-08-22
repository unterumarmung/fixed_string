#include <fixed_string.hpp>
#include <iostream>

int main() {
    constexpr fixstr::fixed_string s = "hello";

    std::cout << std::boolalpha;
    std::cout << s.starts_with("he") << '\n';
    std::cout << s.starts_with('h') << '\n';
    std::cout << s.starts_with("ll") << '\n';
}

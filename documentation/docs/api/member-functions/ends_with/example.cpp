#include <fixed_string.hpp>
#include <iostream>

int main() {
    constexpr fixstr::fixed_string s = "hello";

    std::cout << std::boolalpha;
    std::cout << s.ends_with("lo") << '\n';
    std::cout << s.ends_with('o') << '\n';
    std::cout << s.ends_with("he") << '\n';
}

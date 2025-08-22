#include <fixed_string.hpp>
#include <iostream>

int main() {
    constexpr fixstr::fixed_string s = "bananas";

    std::cout << std::boolalpha;
    std::cout << s.contains("nan") << '\n';
    std::cout << s.contains('b') << '\n';
    std::cout << s.contains("pear") << '\n';
}

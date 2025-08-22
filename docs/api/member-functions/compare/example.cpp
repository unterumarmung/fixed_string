#include <fixed_string.hpp>
#include <iostream>

int main() {
    constexpr fixstr::fixed_string a = "abc";
    constexpr fixstr::fixed_string b = "abd";

    std::cout << a.compare(b) << '\n';
    std::cout << a.compare("abc") << '\n';
    std::cout << b.compare(a) << '\n';
}

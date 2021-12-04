#include <fixed_string.hpp>
#include <iostream>
#include <string>

template <auto n>
void print(auto s) {
    if constexpr (n == decltype(s)::npos) {
        std::cout << "not found\n";
    } else {
        std::cout << "found: " << s.template substr<n>() << '\n';
    }
}

int main() {
    constexpr fixstr::fixed_string s = "This is a string";

    // search from beginning of string
    constexpr auto n1 = s.find("is");
    print<n1>(s);

    // search from position 5
    constexpr auto n2 = s.find("is", 5);
    print<n2>(s);

    // find a single character
    constexpr auto n3 = s.find('a');
    print<n3>(s);

    // find a single character
    constexpr auto n4 = s.find('q');
    print<n4>(s);
}
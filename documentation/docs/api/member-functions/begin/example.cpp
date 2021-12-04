#include <fixed_string.hpp>
#include <iostream>
#include <string>
#include <utility>

int main()
{
    fixstr::fixed_string s = "Exemplar";
    *s.begin() = 'e';
    std::cout << s << '\n';

    auto i1 = s.cbegin();
    std::cout << *i1 << '\n';
    // *i1 = 'E'; // error: i1 is a constant iterator

    auto i2 = std::as_const(s).begin();
    std::cout << *i2 << '\n';
    // *i2 = 'E'; // error: i2 is a constant iterator
}
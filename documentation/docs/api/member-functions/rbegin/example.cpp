#include <algorithm>
#include <fixed_string.hpp>
#include <iostream>
#include <iterator>
#include <string>

int main()
{
    fixstr::fixed_string s("Exemplar!");
    *s.rbegin() = 'y';
    std::cout << s << '\n'; // "Exemplary"

    std::string c;
    std::copy(s.crbegin(), s.crend(), std::back_inserter(c));
    std::cout << c << '\n'; // "yralpmexE"
}
#include <algorithm>
#include <fixed_string.hpp>
#include <iostream>

int main()
{
    fixstr::fixed_string s = "Exemparl";
    std::next_permutation(s.begin(), s.end());
    std::cout << s << '\n'; // "Exemplar"
}
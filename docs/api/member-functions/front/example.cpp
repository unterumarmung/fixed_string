// clang-format off
#include <fixed_string.hpp>
#include <iostream>

int main()
{
    {
        fixstr::fixed_string s = "Exemplary";
        char& f = s.front();
        f = 'e';
        std::cout << s << '\n'; // "exemplary"
    }

    {
        constexpr const fixstr::fixed_string c = "Exemplary";
        char const& f = c.front();
        std::cout << &f << '\n'; // "Exemplary"
    }
}
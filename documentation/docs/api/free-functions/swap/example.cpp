#include <fixed_string.hpp>
#include <iostream>

int main()
{
    fixstr::fixed_string<5> first = "hello";
    fixstr::fixed_string<5> second = "world";

    fixstr::swap(first, second);

    std::cout << "first: " << first.c_str() << '\n';
    std::cout << "second: " << second.c_str() << '\n';
}

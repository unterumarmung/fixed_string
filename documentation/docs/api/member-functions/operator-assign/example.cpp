#include <fixed_string.hpp>
#include <iostream>

int main()
{
    fixstr::fixed_string<5> hello, world;
    hello = "Hello";
    world = "World";
    std::cout << hello << ", " << world;
}
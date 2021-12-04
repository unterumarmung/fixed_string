#include <fixed_string.hpp>
#include <iostream>

int main()
{
    fixstr::fixed_string    hello_world = "Hello, world!";
    fixstr::fixed_string<0> no_hello_world;

    std::cout << std::boolalpha;
    std::cout << "hello_world.empty(): " << hello_world.empty() << '\n';
    std::cout << "no_hello_world.empty(): " << no_hello_world.empty() << '\n';
}
#include <cctype>
#include <fixed_string.hpp>
#include <iostream>

int main()
{
    fixstr::fixed_string str = "Hello, World!";
    std::cout << str << std::endl;

    str[0] = std::tolower(str[0]);
    str[7] = std::tolower(str[7]);

    std::cout << str << std::endl;

    // [[maybe_unused]] const auto foo = str[25]; // Undefined behaviour: the index is out of range
}
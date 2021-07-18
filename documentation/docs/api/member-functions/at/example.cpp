#include <fixed_string.hpp>
#include <iostream>

int main()
{
    fixstr::fixed_string str = "Hello, World!";
    try
    {
        std::cout << str.at(0) << std::endl;
        std::cout << str.at(25) << std::endl;
    }
    catch (std::exception& error)
    {
        std::cout << error.what() << std::endl;
    }
}
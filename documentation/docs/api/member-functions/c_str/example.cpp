#include <fixed_string.hpp>
#include <cstring>
#include <iostream>

int main()
{
    constexpr const fixstr::fixed_string hello_world = "Hello, world!";
    const char* const hw_data = hello_world.c_str();
    
    std::cout << "Terminating character: ";
    if (const char terminating_character = *(hw_data + hello_world.size()); terminating_character == '\0') {
        std::cout << R"(\0)";
    } else {
        std::cout << terminating_character;
    }
    std::cout << std::endl;
    
    constexpr const fixstr::fixed_string answer = "42";
    std::cout << "The answer to The Ultimate Question of Life, the Universe, and Everything: "
              << std::atoi(answer.c_str()) << '\n';
}
#include <fixed_string.hpp>
#include <cstring>
#include <iostream>

int main()
{
    constexpr const fixstr::fixed_string hw1 = "Hello, world!";
    constexpr const fixstr::fixed_string hw2 = hw1;
    constexpr const fixstr::fixed_string hcxx = "Hello, C++!";

    std::cout << std::boolalpha;
    std::cout << "hw1 == hw2: " << std::strcmp(hw1.c_str(), hw2.c_str()) << '\n';
    std::cout << "hw1 != hcxx: " << std::strcmp(hw1.c_str(), hcxx.c_str()) << '\n';
}
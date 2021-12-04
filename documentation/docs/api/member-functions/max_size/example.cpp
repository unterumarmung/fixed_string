#include <fixed_string.hpp>
#include <iostream>

int main()
{
    fixstr::fixed_string s = "Hello, world!";
    std::cout << "Maximum size of the 'fixed_string' is " << s.max_size() << "\n";
}
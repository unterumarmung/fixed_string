#include <fixed_string.hpp>
#include <iostream>
#include <memory>

int main()
{
    std::cout << std::boolalpha;
    {
        fixstr::fixed_string hw_s = "hello, world!";
        char* const f = std::addressof(hw_c.front());
        std::cout << "f == data(): " << (f == hw_c.data()) << '\n'; 
        *f = 'H';
        std::cout << hw_s.data() << '\n'; // Hello, world!
    }

    {
        constexpr const fixstr::fixed_string hw_c = "Hello, world!";
        const char* const f = std::addressof(hw_c.front());
        std::cout << "f == data(): " << (f == hw_c.data()) << '\n'; 
    }
}
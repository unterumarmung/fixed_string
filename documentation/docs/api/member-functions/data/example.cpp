#include <fixed_string.hpp>
#include <iostream>
#include <memory>

int main()
{
    {
        fixstr::fixed_string hw_s = "hello, world!";
        char* const f = std::addressof(hw_s.front());
        *f = 'H';
        std::cout << "data() after modification: " << hw_s.data() << '\n'; // Hello, world!
    }

    {
        constexpr const fixstr::fixed_string hw_c = "Hello, world!";
        const char* const f = std::addressof(hw_c.front());
        std::cout << std::boolalpha << "f == data(): " << (f == hw_c.data()) << '\n'; 
    }
}
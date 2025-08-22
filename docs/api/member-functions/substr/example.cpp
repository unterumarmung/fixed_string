#include <fixed_string.hpp>
#include <iostream>
#include <string>

int main()
{
    constexpr fixstr::fixed_string a = "0123456789abcdefghij";

    // count is npos, returns [pos, size())
    constexpr auto sub1 = a.template substr<10>();
    std::cout << sub1 << '\n';

    // both pos and pos + count are within bounds, returns [pos, pos+count)
    constexpr auto sub2 = a.template substr<5, 3>();
    std::cout << sub2 << '\n';

    // pos is within bounds, pos+count is not, returns [pos, size())
    constexpr auto sub4 = a.template substr<a.size() - 3, 50>();
    // this is effectively equivalent to
    // constexpr auto sub4 = a.substr<17, 3>();
    // since a.size() == 20, pos == a.size() - 3 == 17, and a.size() - pos == 3

    std::cout << sub4 << '\n';

    // pos is out of bounds, compile-time error
    // constexpt auto sub5 = a.substr<a.size() + 3, 50>();
}
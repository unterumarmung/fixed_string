// clang-format off
#include <fixed_string.hpp>
#include <iostream>
 
int main()
{
  {
    fixstr::fixed_string s = "Exemplary";
    char& back = s.back();
    back = 's';
    std::cout << s << '\n'; // "Exemplars"
  }
 
  {
    constexpr const fixstr::fixed_string c = "Exemplary";
    char const& back = c.back();
    std::cout << back << '\n'; // 'y'
  }
}
# fixed_string
C++ library that provides a `basic_fixed_string` template that combines `std::array` fixed-size semantic and `std::string` semantic together

## Features

* C++17 or higher
* Header-only
* Dependency-free
* No dynamic allocations
* Fully constexpr
* Can be used as class non-type template parameter __(since C++20)__

## Examples

* Construction
```cpp
constexpr fixstr::fixed_string foo = "foo";
```

* Concatenation
```cpp
using namespace fixstr;
constexpr fixed_string first = "Hello, ";
constexpr fixed_string second = "World!";
constexpr auto result = first + second; // "Hello, World!"
```

* Comparison
```cpp
using namespace fixstr;
constexpr fixed_string first = "Hello, ";
constexpr fixed_string second = "World!";
static_assert(first == second); // false
static_assert(first != second); // true
static_assert(first < second);  // true
static_assert(first <= second); // true
static_assert(first > second);  // false 
static_assert(first >= second); // false
static_assert(first <=> second != 0); // true
```

* Non-type template parameter
```cpp
template <fixstr::fixed_string Foo>
void bar()
{
    static_assert(Foo == "foo"sv);
}

void foo()
{
    bar<"foo">();
}
```

## Integration
Since it's a header only library, you need just copy `fixed_string.hpp` to your project.

If you are using [vcpkg](https://github.com/Microsoft/vcpkg/) on your project for external dependencies, then you can use the [**fixed-string** package](https://github.com/microsoft/vcpkg/tree/master/ports/fixed-string).

If you are using Conan on your project for external dependencies, then you can use the Conan recipe located in the root of the repository.

## Compiler compatibility
* GCC >= 7.3
* Clang >= 5
* ICC >= 19.0.1
* MSVC >= 14.28 / Visual Studio 2019 (I don't have access to older VS versions right now, so it can work on older versions too)

**Using `basic_fixed_string` as class non-type template parameter full available in GCC >= 10**

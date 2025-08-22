# fixed_string
C++ library that provides a `basic_fixed_string` template combining `std::array`'s fixed-size semantics with `std::string` semantics.

## Features

* C++17 or higher
* Header-only
* Dependency-free
* No dynamic allocations
* Fully `constexpr`
* Can be used as a class non-type template parameter *(since C++20)*

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
Since it's a header-only library, you just need to copy `fixed_string.hpp` to your project.

If you are using [vcpkg](https://github.com/Microsoft/vcpkg/) for external dependencies, you can use the [*fixed-string* package](https://github.com/microsoft/vcpkg/tree/master/ports/fixed-string).

If you are using Conan for external dependencies, you can use the Conan recipe located in the root of the repository.

## Compiler compatibility
* GCC >= 7.3
* Clang >= 5
* ICC >= 19.0.1
* MSVC >= 14.28 / Visual Studio 2019 (I don't have access to older versions, so it might work on them too)

**Using `basic_fixed_string` as a class non-type template parameter is fully available in GCC >= 10 and VS 2019 16.9 or newer**

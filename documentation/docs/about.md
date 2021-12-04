---
sidebar_position: 1
---

# About

This library provides `basic_fixed_string` class template to enable fixed-size `std::array` semantic with standard-like string semantic.

## Features

* C++17 or higher
* Header-only
* Dependency-free

:::caution Warning
Dependencies can be added later to enable lower C++ standards support.
:::

* No dynamic allocations
* `constexpr` as much as possible
* Can be used as class non-type template parameter *(since C++20)*

## Possible usages

* ## Make your own eDSL with C++20's class non-type template parameter feature

[CTRE library](https://github.com/hanickadot/compile-time-regular-expressions) uses similar class to make regular expressions in C++ more easy to use:
```cpp
std::optional<std::string_view> extract_number(std::string_view s) noexcept {
    if (auto m = ctre::match<"[a-z]+([0-9]+)">(s)) {
        return m.get<1>().to_view();
    } else {
        return std::nullopt;
    }
}
```

* ## Make more concise APIs

For example, before `fixed_string` if you needed to implement MD5 hash function, you'd write something like this:
```cpp
std::string hash_md5(std::string_view string);
```
This solution has 2 downsides: 
* it can allocate
* it can return a string that is not 16 bytes

With `fixed_string` these 2 problems are solved:
```cpp
fixstr::fixed_string<16> hash_md5(std::string_view string);
```

* ## Use in a free-standing environment

Returning to the example with the hash function: the implementation with `std::string` as the return type has one more downside which is a consequence of possible allocations - it cannot be used in free-standing environments where is no dynamic memory.

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

**Using `basic_fixed_string` as class non-type template parameter full available in GCC >= 10 and VS 2019 16.9 or newer**

---
sidebar_position: 1
sidebar_label: basic_fixed_string
---

# Class template `basic_fixed_string`

##### Defined in header [`<fixed_string.hpp>`](https://github.com/unterumarmung/fixed_string/blob/master/include/fixed_string.hpp)

```cpp
template<
    typename TChar, 
    std::size_t N, 
    typename TTraits = std::char_traits<TChar>
> struct basic_fixed_string;
```

The class template `basic_fixed_string` describes objects that can store a sequence consisting of a fixed number of arbitrary `char`-like objects with the first element of the sequence at position zero. 

Several typedefs for common character types are provided:

| Type                         | Definition                                |               |
| ---------------------------- | ----------------------------------------- | ------------- |
| `fixstr::fixed_string<N>`    | `fixstr::basic_fixed_string<char, N>`     |               |
| `fixstr::fixed_u8string<N>`  | `fixstr::basic_fixed_string<char8_t, N>`  | *since C++20* |
| `fixstr::fixed_u16string<N>` | `fixstr::basic_fixed_string<char16_t, N>` |               |
| `fixstr::fixed_u32string<N>` | `fixstr::basic_fixed_string<char32_t, N>` |               |
| `fixstr::fixed_wstring<N>`   | `fixstr::basic_fixed_string<wchar_t, N>`  |               |

:::info NOTE
In actual implementation, these are not the type aliases. Unfortunately, early GCC versions that are supported cNTTP couldn't automatically deduct `size_t` template parameter when an alias was used in cNTTP. So, the actual implementation is that every "typedef" is actually a separate class inherited from `basic_fixed_string`. 
:::

### Template parameters

| Name      | Description                                                                                                                                                                                                                                                                   |
| --------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `TChar`   | character type                                                                                                                                                                                                                                                                |
| `N`       | string size                                                                                                                                                                                                                                                                   |
| `TTraits` | [`CharTraits`](https://en.cppreference.com/w/cpp/named_req/CharTraits) class specifying the operations on the character type. Like for `std::basic_string` or `std::basic_string_view`, `TTraits::char_type` must name the same type as `TChar` or the program is ill-formed. |

### Member types

| Member type              | Definiton                                                                                                                 |
| ------------------------ | ------------------------------------------------------------------------------------------------------------------------- |
| `traits_type`            | `TTraits`                                                                                                                 |
| `value_type`             | `TChar`                                                                                                                   |
| `pointer`                | `value_type*`                                                                                                             |
| `const_pointer`          | `const value_type*`                                                                                                       |
| `reference`              | `value_type&`                                                                                                             |
| `iterator`               | [`LegacyRandomAccessIterator`](https://en.cppreference.com/w/cpp/named_req/RandomAccessIterator) to `value_type`          |
| `const_iterator`         | Constant [`LegacyRandomAccessIterator`](https://en.cppreference.com/w/cpp/named_req/RandomAccessIterator) to `value_type` |
| `reverse_iterator`       | [`std::reverse_iterator<iterator>`](https://en.cppreference.com/w/cpp/iterator/reverse_iterator)                          |
| `const_reverse_iterator` | [`std::reverse_iterator<const_iterator>`](https://en.cppreference.com/w/cpp/iterator/reverse_iterator)                    |
| `size_type`              | `size_t`                                                                                                                  |
| `difference_type`        | `ptrdiff_t`                                                                                                               |
| `string_view_type`       | `std::basic_string_view<value_type, traits_type>`                                                                         |

### Member functions

#### Constructors and assignment

| Name                                               | Description                       |
| -------------------------------------------------- | --------------------------------- |
| [*(constructor)*](./member-functions/constructors) | Constructs a `basic_fixed_string` |
| [`operator=`](./member-functions/operator-assign)  | assigns values to the string      |

#### Element access

| Name                                           | Description                                   |
| ---------------------------------------------- | --------------------------------------------- |
| [`operator[]`](./member-functions/operator-at) | access specified element                      |
| [`at`](./member-functions/at)                  | access specified element with bounds checking |
| [`front`](./member-functions/front)            | accesses the first character                  |
| [`back`](./member-functions/back)              | accesses the last character                   |

#### Iterators

| Name                                                  | Description                                 |
| ----------------------------------------------------- | ------------------------------------------- |
| [`begin` <br/> `cbegin`](./member-functions/begin)    | returns an iterator to the beginning        |
| [`end` <br/> `cend`](./member-functions/end)          | returns an iterator to the end              |
| [`rbegin` <br/> `crbegin`](./member-functions/rbegin) | returns a reverse iterator to the beginning |
| [`rend` <br/> `crend`](./member-functions/rend)       | returns a reverse iterator to the end       |

#### Capacity

| Name                                             | Description                              |
| ------------------------------------------------ | ---------------------------------------- |
| [`empty`](./member-functions/empty)              | checks whether the fixed string is empty |
| [`size` <br/> `length`](./member-functions/size) | returns the number of characters         |
| [`max_size`](./member-functions/max_size)        | returns the maximum number of characters |


#### Operations

| Name                                 | Description         |
| ------------------------------------ | ------------------- |
| [`empty`](./member-functions/substr) | returns a substring |

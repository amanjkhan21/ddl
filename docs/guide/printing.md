# Printing

DDL provides stream insertion support through `operator<<`, allowing `ddl::value`, `ddl::array`, and `ddl::object` to be printed directly using C++ output streams.

## Printing Values

Use `std::cout` to print a `ddl::value`.

```cpp
#include "ddl.h"

#include <iostream>

int main() {
    ddl::value number = 42;
    ddl::value text = "Hello, DDL!";
    ddl::value flag = true;
    ddl::value empty = nullptr;

    std::cout << number << '\n';
    std::cout << text << '\n';
    std::cout << flag << '\n';
    std::cout << empty << '\n';

    return 0;
}
```

**Output:**

```text
42
"Hello, DDL!"
true
nullptr
```

## Printing Arrays

Arrays are printed recursively, with their elements enclosed in square brackets.

```cpp
ddl::array values = {
    42,
    "Hello",
    true,
    nullptr
};

std::cout << values << '\n';
```

**Output:**

```text
[
  42,
  "Hello",
  true,
  null
]
```

## Printing Objects

Objects are printed with their keys and values enclosed in curly braces.

```cpp
ddl::object person = {
    {"name", "Alice"},
    {"age", 25},
    {"active", true}
};

std::cout << person << '\n';
```

**Output:**

```text
{
  "active": true,
  "age": 25,
  "name": "Alice"
}
```

Since `ddl::object` uses `std::unordered_map`, the order of entries is unspecified and may vary.

## Printing Nested Collections

DDL recursively prints arrays and objects, including nested structures.

```cpp
ddl::value data = ddl::object{
    {"name", "Alice"},
    {"scores", ddl::array{95, 88, 92}},
    {"active", true}
};

std::cout << data << '\n';
```

**Output:**

```text
{
  "active": true,
  "scores": [
    95,
    88,
    92
  ],
  "name": "Alice"
}
```

Object entry order is unspecified.

## Formatting Behavior

DDL uses the following formatting conventions:

- **Strings** are enclosed in double quotes.
- **Characters** are enclosed in single quotes.
- **Arrays** are enclosed in square brackets.
- **Objects** are enclosed in curly braces.
- **Boolean values** are printed as `true` or `false`.
- **Null values** are printed as `null`.
- **Nested collections** are printed recursively.

## Important Notes

- Printing is intended for readable output and debugging.
- String and character contents are quoted but not escaped.
- Object entry order is unspecified.
- DDL printing is not a JSON serialization format.

Continue to [Exceptions](./exceptions) to learn about the exceptions raised by DDL operations.

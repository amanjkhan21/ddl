# DDL — Dynamic Data Library for C++

DDL (Dynamic Data Library) is a lightweight C++17 library for storing and working with dynamic data.

It provides a `ddl::value` type that can hold different data types, including null, booleans, numbers, characters, strings, arrays, and objects. Arrays and objects can also contain nested values, allowing you to build flexible data structures.

## Features

- Supports null, boolean, numeric, character, and string values.
- Supports nested arrays and objects.
- Provides type checking through `is<T>()`.
- Provides type conversion through `as<T>()`.
- Supports recursive pretty printing.
- Built with C++17.
- Available as a single-header distribution.

## Installation

DDL provides a single-header distribution for easy integration into your projects.

1. Download `ddl.h` from the `dist/` directory.
2. Place `ddl.h` in your project's include directory.
3. Include the header in your C++ source file:

```cpp
#include "ddl.h"
```

DDL requires **C++17 or later**.

## Quick Start

The following example demonstrates how to create values of different types and combine them into an object containing an array.

```cpp
#include "ddl.h"
#include <iostream>

int main() {
    ddl::value name = "Alice";
    ddl::value age = 20;
    ddl::value active = true;

    ddl::array hobbies = {
        "Reading",
        "Programming",
        "Gaming"
    };

    ddl::object person = {
        {"name", name},
        {"age", age},
        {"active", active},
        {"hobbies", hobbies}
    };

    ddl::value data = person;

    std::cout << data << '\n';

    return 0;
}
```

**Output:**

```text
{
  "hobbies": [
    Reading,
    Programming,
    Gaming
  ],
  "active": true,
  "age": 20,
  "name": Alice
}
```

_Note: Object property order may vary because DDL uses `std::unordered_map`._

## Project Structure

```text
DDL/
├── include/
│   └── ddl/
│       └── value.hpp
├── src/
│   └── value.cpp
├── tests/
│   ├── main.cpp
│   ├── value_conversions.cpp
│   ├── value_constructors.cpp
│   ├── value_type_checker.cpp
│   ├── value_printer.cpp
│   ├── value_copy_move.cpp
│   ├── value_arrays.cpp
│   └── value_objects.cpp
├── dist/
│   └── ddl.h
├── LICENSE
├── README.md
├── .gitignore
└── sources.txt
```

## Building from Source

DDL requires a C++17-compatible compiler.

To compile the library and its tests using GCC, run the following command from the project root directory:

```bash
g++ -std=c++17 -Iinclude @sources.txt -o value_tests
```

Run the test executable:

```bash
./value_tests
```

## Supported Types

DDL supports the following types:

| Type                            | C++ Type         |
| ------------------------------- | ---------------- |
| Null                            | `std::nullptr_t` |
| Boolean                         | `bool`           |
| 32-bit integer                  | `std::int32_t`   |
| 64-bit integer                  | `std::int64_t`   |
| Single-precision floating point | `float`          |
| Double-precision floating point | `double`         |
| Character                       | `char`           |
| String                          | `std::string`    |
| Array                           | `ddl::array`     |
| Object                          | `ddl::object`    |

Arrays and objects can contain other `ddl::value` instances, allowing you to create nested dynamic data structures.

## Type Checking

Use `is<T>()` to check whether a `ddl::value` currently holds a specific type.

```cpp
#include "ddl.h"
#include <iostream>

int main() {
    ddl::value data = 42;

    std::cout << std::boolalpha;
    std::cout << data.is<std::int32_t>() << '\n';
    std::cout << data.is<std::string>() << '\n';

    return 0;
}
```

**Output:**

```text
true
false
```

`is<T>()` checks the stored type without performing a conversion.

## Type Conversion

Use `as<T>()` to retrieve a value as a specified C++ type.

```cpp
#include "ddl.h"
#include <iostream>

int main() {
    ddl::value number = 42;

    int integer = number.as<int>();
    double decimal = number.as<double>();
    std::string text = number.as<std::string>();

    std::cout << integer << '\n';
    std::cout << decimal << '\n';
    std::cout << text << '\n';

    return 0;
}
```

**Output:**

```text
42
42
42
```

`as<T>()` performs supported conversions when possible. If a conversion is unsupported or a numeric value is outside the target type's representable range, an exception is thrown.

For arrays and objects, `as<ddl::array>()` and `as<ddl::object>()` return copies of the stored collections.

## Arrays and Objects

DDL provides `ddl::array` and `ddl::object` for storing collections of dynamic values.

### Arrays

Arrays can store values of different types, including other arrays and objects.

```cpp
#include "ddl.h"
#include <iostream>

int main() {
    ddl::array data = {
        42,
        "Hello",
        true,
        3.14,
        ddl::array({'A', false}),
        ddl::object({{"Name", "Bob"}, {"age", 20}})
    };

    ddl::value val = data;

    std::cout << val << '\n';

    return 0;
}
```

**Output:**

```text
[
  42,
  Hello,
  true,
  3.14,
  [
    A,
    false
  ],
  {
    "age": 20,
    "Name": Bob
  }
]
```

### Objects

Objects store key-value pairs using `std::string` keys. Their values can be of different types, including arrays and other objects.

```cpp
#include "ddl.h"
#include <iostream>

int main() {
    ddl::object person = {
        {"name", "Alice"},
        {"age", 20},
        {"active", true},
        {"scores", ddl::array({9, 8, 7.5, nullptr})}
    };

    ddl::value value = person;

    std::cout << value << '\n';

    return 0;
}
```

**Output:**

```text
{
  "scores": [
    9,
    8,
    7.5,
    null
  ],
  "active": true,
  "age": 20,
  "name": Alice
}
```

_Note: Object property order is not guaranteed because DDL uses `std::unordered_map`._

## Printing

DDL supports printing `ddl::value` instances using the stream insertion operator (`<<`).

Values are printed recursively, so nested arrays and objects are formatted with indentation.

```cpp
#include "ddl.h"
#include <iostream>

int main() {
    ddl::value data = ddl::object({
        {"name", "Alice"},
        {"scores", ddl::array({95, 88, 92})}
    });

    std::cout << data << '\n';

    return 0;
}
```

**Output:**

```text
{
  "scores": [
    95,
    88,
    92
  ],
  "name": Alice
}
```

String values are printed without quotation marks, while object keys are printed with quotation marks.

## Exceptions

DDL uses standard C++ exceptions to report conversion errors.

| Exception               | When it is thrown                                                                               |
| ----------------------- | ----------------------------------------------------------------------------------------------- |
| `std::invalid_argument` | When a conversion is unsupported or the stored value cannot be converted to the requested type. |
| `std::out_of_range`     | When a numeric conversion produces a value outside the target type's representable range.       |

**Example:**

```cpp
#include "ddl.h"
#include <iostream>
#include <stdexcept>

int main() {
    ddl::value data = "Hello";

    try {
        int number = data.as<int>();
    }
    catch (const std::invalid_argument& e) {
        std::cout << "Invalid conversion: " << e.what() << '\n';
    }

    return 0;
}
```

**Output:**

```text
Invalid conversion: ...
```

_Note: The exact exception message may vary depending on the implementation._

## License

DDL is licensed under the [MIT License](LICENSE).

You are free to use, modify, and distribute the library under the terms of the license.

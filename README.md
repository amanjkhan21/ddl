# DDL — Dynamic Data Library for C++

DDL (Dynamic Data Library) is a lightweight, header-only C++17 library for storing, accessing, and working with dynamically typed data.

At its core is `ddl::value`, a type-safe container built around `std::variant` that can hold null, boolean, numeric, character, string, array, and object values.

DDL provides runtime type checking, value conversion, direct reference access, support for nested data structures, and recursive pretty printing.

## Features

- **Dynamic value storage** — Store null, boolean, numeric, character, string, array, and object values in a single `ddl::value`.
- **Nested data structures** — Combine arrays and objects to represent arbitrarily nested data.
- **Runtime type checking** — Check the currently stored type using `is<T>()`.
- **Value conversion** — Retrieve values in supported types using `as<T>()`.
- **Direct reference access** — Access and modify stored values using `ref<T>()`.
- **Recursive pretty printing** — Print nested values using the stream insertion operator (`operator<<`).
- **Type safety** — Uses `std::variant` for internal value storage.
- **Exception handling** — Uses standard C++ exceptions for unsupported conversions and out-of-range numeric conversions.
- **Header-only** — Integrate DDL by including a single header.
- **C++17** — Requires a C++17-compatible compiler and standard library.

## Documentation

For detailed guides and examples, visit the [**DDL Documentation**](https://amanjkhan21.github.io/ddl/).

## Installation

DDL is distributed as a single-header library.

### Single-header distribution

1. Download `ddl.h` from the `dist/` directory.
2. Place the header in your project's include directory.
3. Include it in your source files:

```cpp
#include "ddl.h"
```

Compile your program with C++17 enabled:

```bash
g++ -std=c++17 main.cpp -o app
```

No additional DDL source files or linking steps are required.

## Quick Start

DDL allows a single `ddl::value` to store different types of data. Values can be reassigned at runtime, while arrays and objects can combine different types and contain nested structures.

```cpp
#include "ddl.h"

#include <iostream>
#include <string>

int main() {
    // A value can hold different types at runtime.
    ddl::value data = 42;
    std::cout << data << '\n';

    data = "Hello, DDL!";
    std::cout << data << '\n';

    data = true;
    std::cout << data << '\n';

    // Arrays can contain mixed types and nested collections.
    ddl::value items = ddl::array{
        42,
        "Hello",
        true,
        3.14,
        ddl::object{
            {"name", "Alice"},
            {"active", false}
        }
    };

    // Objects can hold and organize dynamic values.
    ddl::value config = ddl::object{
        {"application", "MyApp"},
        {"version", 2.0},
        {"debug", true},
        {"items", items}
    };

    // Modify stored values directly.
    ddl::object& settings = config.ref<ddl::object>();
    settings["version"] = "2.0.0";
    settings["debug"] = false;

    std::cout << config << '\n';

    return 0;
}
```

**Output:**

```text
42
"Hello, DDL!"
true
{
  "items": [
    42,
    "Hello",
    true,
    3.14,
    {
      "active": false,
      "name": "Alice"
    }
  ],
  "debug": false,
  "version": "2.0.0",
  "application": "MyApp"
}
```

_Note: Object property order may vary because DDL uses `std::unordered_map`. The printed representation is intended for readable output and is not a JSON serialization format._

## Project Structure

```text
ddl/
├── .github/           # GitHub workflows
├── dist/              # Single-header distribution
├── docs/              # Documentation website
├── include/           # Public header files
├── src/               # Library source files
├── tests/             # Test cases
├── LICENSE            # Project license
├── package.json       # Documentation dependencies and scripts
├── package-lock.json  # Locked documentation dependencies
├── README.md          # Project overview and instructions
└── sources.txt        # Source files list for compilation
```

## Building and Running Tests

DDL includes a test suite covering its core functionality.

From the project root, compile the library and its tests using GCC:

```bash
g++ -Iinclude @sources.txt -o test
```

Run the test executable:

```bash
./test
```

Alternatively, compile and run the tests in a single command:

```bash
g++ -Iinclude @sources.txt -o test && ./test
```

**Expected Output:**

```text
[PASS] Value conversions
[PASS] Value constructors
[PASS] Value type checker
[PASS] Value printer
[PASS] Value copy and move semantics
[PASS] Value arrays
[PASS] Value objects
[PASS] Value references

All tests passed successfully!
```

A successful execution confirms that all test cases have passed.

## License

DDL is licensed under the [MIT License](LICENSE).

You are free to use, modify, and distribute the library under the terms of the license.

# Type Checking

DDL provides the `is<T>()` method to check whether a `ddl::value` currently stores a specific type.

## Using `is<T>()`

The `is<T>()` method returns `true` if the stored type exactly matches `T`, otherwise it returns `false`.

```cpp
#include "ddl.h"

#include <iostream>
#include <cstdint>

int main() {
    ddl::value data = 42;

    std::cout << data.is<std::int32_t>() << '\n'; // 1
    std::cout << data.is<double>() << '\n';       // 0

    return 0;
}
```

## Exact Type Matching

Type checking does not perform conversions.

For example, an integer value can be converted to a `double` using `as<double>()`, but `is<double>()` still returns `false` if the stored value is an integer.

```cpp
ddl::value data = 42;

if (data.is<std::int32_t>()) {
    std::cout << "Stored as int32_t\n";
}

if (data.is<double>()) {
    std::cout << "Stored as double\n";
}
```

Only the first condition is true.

## Checking Collection Types

You can also check whether a value stores an array or an object.

```cpp
ddl::value arrayValue = ddl::array{1, 2, 3};
ddl::value objectValue = ddl::object{
    {"name", "Alice"}
};

std::cout << arrayValue.is<ddl::array>() << '\n';   // 1
std::cout << objectValue.is<ddl::object>() << '\n'; // 1
```

## Supported Type Checks

You can use `is<T>()` with any type supported by `ddl::value`, including:

- `std::nullptr_t`
- `bool`
- `float`
- `double`
- `char`
- `std::int32_t`
- `std::int64_t`
- `std::string`
- `ddl::array`
- `ddl::object`

Remember that `is<T>()` checks the exact stored type. It does not check whether a value can be converted to that type.

Continue to [Conversions](./conversions) to learn how to retrieve values using `as<T>()`.

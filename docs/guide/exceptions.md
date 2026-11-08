# Exceptions

DDL uses standard C++ exceptions to report errors during value conversion and reference access.

The library primarily uses two exception types:

- `std::invalid_argument`
- `std::out_of_range`

## `std::invalid_argument`

This exception is thrown when an operation requests an unsupported conversion or a reference to a type that does not match the stored type.

### Unsupported Conversion

For example, converting a string to an integer is not supported.

```cpp
ddl::value data = "Hello";

try {
    int number = data.as<int>();
} catch (const std::invalid_argument& error) {
    std::cerr << error.what() << '\n';
}
```

### Reference Type Mismatch

`ref<T>()` requires an exact match with the stored type.

```cpp
ddl::value data = 42;

try {
    double& number = data.ref<double>();
} catch (const std::invalid_argument& error) {
    std::cerr << error.what() << '\n';
}
```

## `std::out_of_range`

This exception is thrown when a numeric conversion produces a value outside the representable range of the requested destination type.

For example, converting a large `double` to `float` can cause a range error.

```cpp
ddl::value data = 1e100;

try {
    float number = data.as<float>();
} catch (const std::out_of_range& error) {
    std::cerr << error.what() << '\n';
}
```

Numeric conversions to `std::int32_t` and `std::int64_t` also perform range checks.

## Handling Exceptions

Use `try` and `catch` blocks to handle errors safely.

```cpp
#include "ddl.h"

#include <iostream>
#include <stdexcept>

int main() {
    ddl::value data = "Hello";

    try {
        int number = data.as<int>();
        std::cout << number << '\n';
    } catch (const std::invalid_argument& error) {
        std::cerr << "Invalid conversion: " << error.what() << '\n';
    } catch (const std::out_of_range& error) {
        std::cerr << "Value out of range: " << error.what() << '\n';
    }

    return 0;
}
```

## Exception Summary

| Exception               | Cause                                                                  |
| ----------------------- | ---------------------------------------------------------------------- |
| `std::invalid_argument` | Unsupported conversion requested through `as<T>()`.                    |
| `std::invalid_argument` | Requested reference type does not match the stored type in `ref<T>()`. |
| `std::out_of_range`     | Numeric conversion exceeds the destination type's representable range. |

## Additional Notes

- `is<T>()` does not throw exceptions; it simply returns whether the stored type matches `T`.
- `as<T>()` can throw `std::invalid_argument` or `std::out_of_range`, depending on the conversion.
- `ref<T>()` can throw `std::invalid_argument` when the requested type does not match.
- Standard collection operations may also throw their own standard-library exceptions, such as `std::out_of_range` when accessing a missing object key with `at()`.

This completes the DDL guide. You can now refer to the individual sections for details about supported types, conversions, references, collections, printing, and exception handling.

# References

DDL provides the `ref<T>()` method to access the value stored inside a `ddl::value` through a reference.

Unlike `as<T>()`, which returns a copy, `ref<T>()` provides direct access to the stored object. This allows you to read or modify the stored value without copying it.

## Mutable References

Use `ref<T>()` on a mutable `ddl::value` to obtain a mutable reference.

```cpp
#include "ddl.h"

#include <iostream>
#include <cstdint>

int main() {
    ddl::value data = 42;

    std::int32_t& number = data.ref<std::int32_t>();

    number = 100;

    std::cout << data << '\n'; // 100

    return 0;
}
```

Changes made through the reference directly affect the value stored in `ddl::value`.

## Const References

For read-only access, use a const reference.

```cpp
ddl::value data = "Hello, DDL!";

const std::string& text = data.ref<std::string>();

std::cout << text << '\n';
```

You can also call `ref<T>()` on a const `ddl::value`, which returns a const reference.

```cpp
const ddl::value data = 42;

const std::int32_t& number = data.ref<std::int32_t>();

std::cout << number << '\n';
```

## Accessing the Entire Value

You can retrieve a reference to the `ddl::value` itself by using `ref<ddl::value>()`.

```cpp
ddl::value data = 42;

ddl::value& value = data.ref<ddl::value>();

value = "Updated value";

std::cout << data << '\n'; // "Updated value"
```

This is useful when you want to modify or replace the entire stored value through a reference.

## References to Arrays and Objects

References also work with `ddl::array` and `ddl::object`.

```cpp
ddl::value data = ddl::array{1, 2, 3};

ddl::array& values = data.ref<ddl::array>();

values.push_back(4);

std::cout << data << '\n';
```

Similarly, you can access and modify an object:

```cpp
ddl::value data = ddl::object{
    {"name", "Alice"}
};

ddl::object& person = data.ref<ddl::object>();

person["age"] = 25;

std::cout << data << '\n';
```

## Type Requirements

The requested type must exactly match the type currently stored in the `ddl::value`.

Unlike `as<T>()`, `ref<T>()` does not perform conversions.

```cpp
ddl::value data = 42;

// Correct: the stored type is std::int32_t
std::int32_t& number = data.ref<std::int32_t>();

// Throws std::invalid_argument: the stored type is not double
// double& decimal = data.ref<double>();
```

If the requested type does not match the stored type, `std::invalid_argument` is thrown.

## Lvalue Requirement

References can only be obtained from lvalue values. Calling `ref<T>()` on a temporary value is not allowed.

```cpp
ddl::value data = 42;

std::int32_t& number = data.ref<std::int32_t>(); // Valid

// Invalid: ref() cannot be called on a temporary
// std::int32_t& temporary = ddl::value{42}.ref<std::int32_t>();
```

This restriction prevents references from being returned to temporary objects that would immediately be destroyed.

## Important Notes

- `ref<T>()` returns a reference to the stored object, not a copy.
- Mutable references allow direct modification of stored values.
- Const references provide read-only access.
- The requested type must exactly match the stored type.
- Type mismatches throw `std::invalid_argument`.
- References cannot be obtained from temporary `ddl::value` objects.
- A reference should not be used after the lifetime of its owning `ddl::value` ends or after an operation invalidates the referenced object.

Continue to [Arrays and Objects](./collections) to explore DDL's collection types in more detail.

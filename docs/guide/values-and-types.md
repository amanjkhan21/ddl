# Values and Types

At the heart of DDL is `ddl::value`, a container that can hold a value of several different types.

Unlike a regular C++ variable, whose type is fixed, a `ddl::value` can store different types of data during its lifetime.

## Supported Types

DDL supports the following types:

| Type          | Description                            |
| ------------- | -------------------------------------- |
| `nullptr_t`   | Represents a null value                |
| `bool`        | Boolean value (`true` or `false`)      |
| `int32_t`     | 32-bit signed integer                  |
| `int64_t`     | 64-bit signed integer                  |
| `float`       | Single-precision floating-point number |
| `double`      | Double-precision floating-point number |
| `char`        | Single character                       |
| `std::string` | Text                                   |
| `ddl::array`  | Collection of DDL values               |
| `ddl::object` | Collection of key-value pairs          |

## The `ddl::value` Type

A `ddl::value` stores exactly one of the supported types at a time.

For example:

```cpp
ddl::value data = 42;
```

The value currently stores an integer. It can later hold something completely different:

```cpp
data = "Hello, DDL!";
```

Now it stores a string instead.

This flexibility is provided by `std::variant`, which manages the currently stored type.

## Type Checking

Use `is<T>()` to determine which type a value currently holds.

```cpp
ddl::value data = 42;

if (data.is<std::int32_t>()) {
    std::cout << "Integer value\n";
}
```

The function returns:

- `true` if the stored type exactly matches `T`.
- `false` otherwise.

For example:

```cpp
ddl::value data = 42;

std::cout << data.is<std::int32_t>() << '\n'; // 1
std::cout << data.is<double>() << '\n';       // 0
```

**Important:** `is<T>()` checks the stored type, not whether the value can be converted to `T`.

For example, an `int32_t` value can be converted to a `double`, but `is<double>()` still returns `false`.

## Arrays and Objects

Two of DDL's supported types are containers:

- `ddl::array` is an alias for `std::vector<ddl::value>`.
- `ddl::object` is an alias for `std::unordered_map<std::string, ddl::value>`.

Because their elements are `ddl::value` instances, both containers can hold different types and support nested structures.

See [Arrays and Objects](/guide/arrays-and-objects) for more details.

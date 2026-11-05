# `ddl::value`

## Overview

`ddl::value` is the core class of DDL. It can store exactly one value at a time from a predefined set of supported types.

Internally, it uses `std::variant` to manage the stored data.

```cpp
namespace ddl {
    class value;

    using type = std::variant<
        std::nullptr_t,
        bool,
        std::int32_t,
        std::int64_t,
        float,
        double,
        char,
        std::string,
        array,
        object
    >;
}
```

A `ddl::value` can change its stored type during its lifetime.

```cpp
ddl::value data = 42;

data = "Hello, DDL!";
```

## Constructors

`ddl::value` provides constructors for all supported types.

| Constructor             | Description                                    |
| ----------------------- | ---------------------------------------------- |
| `value()`               | Constructs a null value                        |
| `value(std::nullptr_t)` | Constructs a null value                        |
| `value(bool)`           | Stores a boolean                               |
| `value(std::int32_t)`   | Stores a 32-bit signed integer                 |
| `value(std::int64_t)`   | Stores a 64-bit signed integer                 |
| `value(float)`          | Stores a single-precision floating-point value |
| `value(double)`         | Stores a double-precision floating-point value |
| `value(char)`           | Stores a character                             |
| `value(std::string)`    | Stores a string                                |
| `value(const char*)`    | Stores a C-style string                        |
| `value(array)`          | Stores an array                                |
| `value(object)`         | Stores an object                               |

### Examples

```cpp
ddl::value a;                       // null
ddl::value b = nullptr;             // null
ddl::value c = true;                // bool
ddl::value d = 42;                  // integer
ddl::value e = 3.14;                // double
ddl::value f = 'A';                 // char
ddl::value g = "Hello";             // string

ddl::value h = ddl::array{1, 2, 3};
ddl::value i = ddl::object{
    {"name", "Alice"},
    {"age", 20}
};
```

## Type Checking — `is<T>()`

Checks whether the currently stored type exactly matches `T`.

### Signature

```cpp
template <typename T>
bool is() const;
```

### Parameters

- `T`: The type to check against.

### Returns

- `true` if the stored type exactly matches `T`.
- `false` otherwise.

### Example

```cpp
ddl::value data = 42;

bool isInteger = data.is<std::int32_t>(); // true
bool isDouble = data.is<double>();       // false
```

`is<T>()` does not perform conversions. It only checks the currently stored type.

## Value Conversion — `as<T>()`

Retrieves the stored value as the requested type, following DDL's conversion rules.

### Signature

```cpp
template <typename T>
T as() const;
```

### Parameters

- `T`: The requested return type.

### Returns

The value converted to `T`, if the conversion is supported.

### Example

```cpp
ddl::value data = 42;

int number = data.as<int>();
double decimal = data.as<double>();
std::string text = data.as<std::string>();
```

### Exceptions

| Exception               | Condition                                                                     |
| ----------------------- | ----------------------------------------------------------------------------- |
| `std::invalid_argument` | The requested conversion is unsupported or invalid                            |
| `std::out_of_range`     | A numeric conversion exceeds the target type's range or cannot be represented |

Unsupported target types are rejected at compile time.

For detailed conversion rules, see [Type Conversions](/guide/type-conversions).

## Copy and Move Semantics

`ddl::value` owns its stored data directly. It supports normal C++ copy and move operations.

### Copying

Copying a `ddl::value` creates an independent copy of its stored data.

```cpp id="3kqj8v"
ddl::value original = ddl::array{10, 20, 30};

ddl::value copied = original;

copied = "Changed";

std::cout << original << '\n';
std::cout << copied << '\n';
```

Output:

```text id="kqj8v2"
[
  10,
  20,
  30
]
Changed
```

Changes to the copied value do not affect the original.

### Moving

`ddl::value` also supports move construction and move assignment.

```cpp
ddl::value original = ddl::array{10, 20, 30};

ddl::value moved = std::move(original);
```

Moving allows the stored data to be transferred rather than copied.

After a move, the original object remains valid but its value should not be relied upon without checking or assigning a new value.

## Summary

- `ddl::value` stores one supported type at a time.
- Constructors accept all supported types.
- `is<T>()` checks the exact stored type.
- `as<T>()` retrieves or converts the stored value.
- Copying creates an independent value.
- Moving transfers the stored data.

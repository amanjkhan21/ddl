# Supported Types

`ddl::value` can store one value at a time from a set of supported C++ types. Internally, it uses `std::variant` to manage these alternatives.

## Primitive Types

The following primitive types are supported:

| C++ Type         | Description                                          |
| ---------------- | ---------------------------------------------------- |
| `std::nullptr_t` | Represents a null value.                             |
| `bool`           | Represents a boolean value.                          |
| `float`          | Represents a single-precision floating-point number. |
| `double`         | Represents a double-precision floating-point number. |
| `char`           | Represents a single character.                       |
| `std::int32_t`   | Represents a 32-bit signed integer.                  |
| `std::int64_t`   | Represents a 64-bit signed integer.                  |
| `std::string`    | Represents a string of characters.                   |

Example:

```cpp
ddl::value nullValue = nullptr;
ddl::value boolean = true;
ddl::value single = 3.14f;
ddl::value decimal = 3.14159;
ddl::value character = 'A';
ddl::value integer = std::int32_t{42};
ddl::value largeInteger = std::int64_t{10000000000LL};
ddl::value text = "Hello, DDL!";
```

## Arrays

`ddl::array` is an alias for `std::vector<ddl::value>`.

Arrays can store values of different supported types and can contain other arrays or objects.

```cpp
ddl::array values = {
    42,
    "Hello",
    true,
    nullptr,
    3.14
};
```

## Objects

`ddl::object` is an alias for `std::unordered_map<std::string, ddl::value>`.

Objects store key-value pairs, where each key is a string and each value is a `ddl::value`.

```cpp
ddl::object person = {
    {"name", "Alice"},
    {"age", 25},
    {"active", true}
};
```

## Nested Data Structures

Arrays and objects can contain one another, allowing you to build nested structures.

```cpp
ddl::object user = {
    {"name", "Alice"},
    {"age", 25},
    {"skills", ddl::array{"C++", "Python"}},
    {"address", ddl::object{
        {"city", "London"},
        {"active", true}
    }}
};

ddl::value data = user;
```

## Type Aliases

DDL provides two collection aliases:

```cpp
using array = std::vector<value>;
using object = std::unordered_map<std::string, value>;
```

Both aliases use `ddl::value` as their element type, so collections can hold any supported value, including nested collections.

Continue to [Type Checking](./type-checking) to learn how to inspect the type currently stored in a `ddl::value`.

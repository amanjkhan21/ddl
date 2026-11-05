# Quick Start

This guide introduces the basic features of DDL through a few examples.

## Creating Values

DDL provides a unified `ddl::value` type that can store several different data types.

```cpp
ddl::value a = 42;
ddl::value b = "Hello, DDL!";
ddl::value c = true;
ddl::value d = nullptr;
```

## Checking Types

Use `is<T>()` to check which type a value currently stores.

```cpp
ddl::value data = 42;

if (data.is<std::int32_t>()) {
    std::cout << "The value stores an int32_t.\n";
}
```

## Converting Values

Use `as<T>()` to retrieve a value in a requested type.

```cpp
ddl::value data = 42;

int number = data.as<int>();
std::string text = data.as<std::string>();
```

Conversions follow DDL's documented conversion rules. Unsupported conversions throw exceptions.

## Working with Arrays

Unlike regular C++ arrays or vectors, a DDL array can contain values of different types.

```cpp
ddl::array values = {
    42,
    "Hello",
    true,
    nullptr,
    ddl::array{10, 20, 30}
};

ddl::value data = values;
```

Each element is a `ddl::value`, so arrays can contain different types and even other arrays or objects.

You can retrieve an array using `as<ddl::array>()`.

## Working with Objects

Objects store key-value pairs using string keys.

```cpp
ddl::object person = {
    {"name", "Alice"},
    {"age", 20},
    {"isStudent", true}
};

ddl::value data = person;
```

Values inside objects can themselves be arrays or other objects.

## Printing Values

Use the stream insertion operator to print a value.

```cpp
ddl::value data = ddl::object{
    {"name", "Alice"},
    {"age", 20},
    {"skills", ddl::array{"C++", "Python"}}
};

std::cout << data << '\n';

```

Output:

```text
{
  "skills": [
    C++,
    Python
  ],
  "age": 20,
  "name": Alice
}
```

DDL recursively formats arrays and objects, making nested data easier to read.

Note: Object property order is unspecified because DDL uses `std::unordered_map`.

## What's Next?

Explore [Values and Types](/guide/values-and-types) to understand the data model in more detail.

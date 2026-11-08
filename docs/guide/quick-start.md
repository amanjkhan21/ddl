# Quick Start

This guide walks through the basics of using DDL, including creating values, checking their types, converting them, accessing stored values by reference, and working with arrays and objects.

## Include DDL

Start by including the DDL header and the standard library headers needed for the example.

```cpp
#include "ddl.h"

#include <iostream>
#include <string>
```

## Creating Values

Use `ddl::value` to store different types of data.

```cpp
ddl::value number = 42;
ddl::value text = "Hello, DDL!";
ddl::value flag = true;
ddl::value empty = nullptr;
```

A `ddl::value` can also be reassigned to hold a different supported type:

```cpp
ddl::value data = 100;

data = "Now I'm a string";
data = false;
```

## Checking Types

Use `is<T>()` to check the exact type currently stored in a value.

```cpp
ddl::value data = 42;

if (data.is<std::int32_t>()) {
    std::cout << "Data stores an integer\n";
}
```

## Converting Values

Use `as<T>()` to retrieve a value in a supported type.

```cpp
ddl::value data = 42;

int number = data.as<int>();
double decimal = data.as<double>();

std::cout << number << '\n';
std::cout << decimal << '\n';
```

Conversions that are not supported throw `std::invalid_argument`. Numeric conversions that exceed the destination type's range throw `std::out_of_range`.

## Accessing Values by Reference

Use `ref<T>()` to access the value stored inside a `ddl::value` through a reference. Unlike `as<T>()`, which returns a value, `ref<T>()` provides direct access to the stored object.

```cpp
ddl::value data = 42;

std::int32_t& number = data.ref<std::int32_t>();

number = 100;

std::cout << data << '\n'; // 100
```

You can also use a const reference when you only need to read the stored value:

```cpp
const ddl::value data = "Hello, DDL!";

const std::string& text = data.ref<std::string>();

std::cout << text << '\n';
```

To obtain a reference to the entire `ddl::value` itself, use `ref<ddl::value>()`:

```cpp
ddl::value data = 42;

ddl::value& value = data.ref<ddl::value>();

value = "Updated value";

std::cout << data << '\n';
```

The requested type must match the exact stored type. If it does not, `ref<T>()` throws `std::invalid_argument`.

References can only be obtained from lvalue values, preventing references from being taken from temporary objects.

## Working with Arrays

`ddl::array` is a dynamic array that stores `ddl::value` elements.

```cpp
ddl::array values = {
    42,
    "Hello",
    true,
    nullptr
};

ddl::value data = values;

std::cout << data << '\n';
```

Arrays can contain other arrays and objects, allowing nested data structures.

## Working with Objects

`ddl::object` stores key-value pairs with string keys and `ddl::value` values.

```cpp
ddl::object person = {
    {"name", "Alice"},
    {"age", 25},
    {"active", true}
};

ddl::value data = person;

std::cout << data << '\n';
```

You can access an object's elements using their keys:

```cpp
std::string name = data.as<ddl::object>().at("name").as<std::string>();

std::cout << name << '\n';
```

## Printing Values

Use `std::cout` to print values directly. DDL supports primitive values, arrays, and objects through the stream insertion operator.

```cpp
ddl::value data = ddl::object{
    {"message", "Hello, DDL!"},
    {"success", true}
};

std::cout << data << '\n';
```

## Complete Example

```cpp
#include "ddl.h"

#include <iostream>
#include <string>

int main() {
    // Create a value
    ddl::value data = 42;

    // Check its type
    if (data.is<std::int32_t>()) {
        std::cout << "Integer: " << data.as<int>() << '\n';
    }

    // Access and modify the stored value by reference
    std::int32_t& number = data.ref<std::int32_t>();
    number = 100;

    std::cout << "Updated integer: " << data << '\n';

    // Reassign it
    data = "Hello, DDL!";
    std::cout << "String: " << data.as<std::string>() << '\n';

    // Create and print an array
    ddl::array values = {1, "two", true};
    std::cout << "Array: " << values << '\n';

    // Create and print an object
    ddl::object person = {
        {"name", "Alice"},
        {"age", 25}
    };
    std::cout << "Object: " << person << '\n';

    return 0;
}
```

This example covers the basic workflow of creating, checking, converting, referencing, and printing DDL values.

Continue to [Supported Types](./types) to see all types that `ddl::value` can store.

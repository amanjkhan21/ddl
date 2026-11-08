# Introduction

DDL (Dynamic Data Library) is a lightweight, header-only C++17 library for storing, accessing, and working with dynamically typed data.

At its core is `ddl::value`, a type-safe container that can hold different types of values at runtime. It is built using `std::variant` and provides a unified interface for working with primitive values and nested data structures.

## Overview

Unlike traditional variables in C++, which have a fixed type, a `ddl::value` can store different supported types throughout its lifetime.

```cpp
#include "ddl.h"

#include <iostream>

int main() {
    ddl::value data = 42;
    std::cout << data << '\n';

    data = "Hello, DDL!";
    std::cout << data << '\n';

    data = true;
    std::cout << data << '\n';

    return 0;
}
```

**Output:**

```text
42
"Hello, DDL!"
true
```

The stored type can be checked at runtime, converted into another supported type, or accessed directly through references.

## Core Components

DDL provides three fundamental types:

### `ddl::value`

The central type of the library. It stores one of the supported value types and provides operations for type checking, conversion, and reference access.

### `ddl::array`

A dynamic array implemented using `std::vector<ddl::value>`.

It can contain values of different types, including other arrays and objects.

### `ddl::object`

A key-value container implemented using `std::unordered_map<std::string, ddl::value>`.

It associates string keys with dynamic values and supports nested data structures.

## Supported Features

DDL provides the following functionality:

- **Dynamic Value Storage** — Store different supported types in a single value container.
- **Type Checking** — Determine the currently stored type using `is<T>()`.
- **Value Conversion** — Retrieve values in supported types using `as<T>()`.
- **Reference Access** — Access stored values through `ref<T>()`.
- **Nested Collections** — Combine arrays and objects to build nested data structures.
- **Recursive Printing** — Print values and nested collections using `operator<<`.

## Requirements

DDL requires:

- A C++17-compatible compiler.

DDL is header-only, so no separate library compilation or linking is required.

## Next Steps

Continue with the [Installation](./installation) guide to integrate DDL into your project, or proceed to the [Quick Start](./quick-start) guide to explore its basic usage.

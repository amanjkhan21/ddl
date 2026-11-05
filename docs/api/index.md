# API Reference

This section documents the public API provided by DDL.

DDL is built around a single core type, `ddl::value`, supported by two container aliases: `ddl::array` and `ddl::object`.

## Namespace

All DDL types and functionality are contained in the `ddl` namespace.

```cpp
#include "ddl.h"

ddl::value data = 42;
```

## Core Types and Functions

| Name          | Description                                              |
| ------------- | -------------------------------------------------------- |
| `ddl::value`  | Stores one of the supported data types                   |
| `ddl::array`  | Alias for `std::vector<ddl::value>`                      |
| `ddl::object` | Alias for `std::unordered_map<std::string, ddl::value>`  |
| `ddl::type`   | Variant containing all supported types                   |
| `operator<<`  | Non-member stream insertion operator for printing values |

## API Overview

### `ddl::value`

The main class of the library.

- Constructors for all supported types.
- `is<T>()` for checking the currently stored type.
- `as<T>()` for retrieving or converting values.

### `ddl::array`

A standard vector of `ddl::value` elements.

### `ddl::object`

A standard unordered map with string keys and `ddl::value` elements.

### `operator<<`

A non-member stream insertion operator that supports printing `ddl::value` instances, including nested arrays and objects.

## Version Information

DDL provides version macros:

```cpp
#define DDL_VERSION_MAJOR 1
#define DDL_VERSION_MINOR 0
#define DDL_VERSION_PATCH 0
```

These macros represent the library version: **1.0.0**.

## Detailed Reference

The following pages provide more information about individual components:

- [Value Class](/api/value)
- [Stream Insertion Operator](/api/stream-operator)

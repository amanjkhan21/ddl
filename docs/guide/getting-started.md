# Getting Started

DDL is a lightweight C++17 library for storing and working with dynamic data.

## Requirements

Before using DDL, make sure you have:

- A C++ compiler with C++17 support.
- The DDL header file.

## Installation

DDL supports a simple single-header distribution.

1. Download `ddl.h` from the project's `dist` directory.
2. Place it somewhere in your project.
3. Include it in your source file:

```cpp
#include "ddl.h"
```

That's it! You can now start using DDL.

## Your First Program

Let's create a simple value and print it.

```cpp
#include "ddl.h"
#include <iostream>

int main() {
    ddl::value data = 42;

    std::cout << data << '\n';

    return 0;
}
```

Output:

```text
42
```

## What's Next?

Continue to the [Quick Start](/guide/quick-start) to explore DDL's values, arrays, and objects.

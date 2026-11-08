# Installation

DDL is a header-only C++17 library. You can integrate it into your project by adding its single header to your include directory.

## Requirements

- A C++17-compatible compiler.

## Install the Single Header

Copy `dist/ddl.h` into your project's include directory.

Include it in your C++ source files:

```cpp
#include "ddl.h"
```

Make sure the directory containing `ddl.h` is available to your compiler's include paths.

## Compile a Test Program

For example, if `ddl.h` is in the `include` directory:

```bash
g++ -std=c++17 -Iinclude main.cpp -o main
```

Run the executable:

```bash
./main
```

On Windows, run:

```bash
main.exe
```

Once DDL is included in your project, you can start using `ddl::value`, `ddl::array`, and `ddl::object`.

Continue to the [Quick Start](./quick-start) guide to see the basic API in action.

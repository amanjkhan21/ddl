# Stream Insertion Operator

DDL provides a non-member stream insertion operator (`operator<<`) for printing `ddl::value` instances.

It supports all stored types, including nested arrays and objects.

## Signature

```cpp
std::ostream& operator<<(std::ostream& os, const value& v);
```

## Parameters

| Parameter | Description               |
| --------- | ------------------------- |
| `os`      | Output stream to write to |
| `v`       | DDL value to print        |

## Returns

Returns a reference to the output stream, allowing chained stream operations.

## Basic Usage

```cpp
ddl::value data = 42;

std::cout << data << '\n';
```

Output:

```text
42
```

## Printing Arrays

```cpp
ddl::value data = ddl::array{10, "Hello", true};

std::cout << data << '\n';
```

Output:

```text
[
  10,
  Hello,
  true
]
```

## Printing Objects

```cpp
ddl::value data = ddl::object{
    {"name", "Alice"},
    {"age", 20}
};

std::cout << data << '\n';
```

Possible output:

```text
{
  "age": 20,
  "name": Alice
}
```

Object property order is unspecified because DDL uses `std::unordered_map`.

## Nested Structures

The operator recursively prints nested arrays and objects.

```cpp
ddl::value data = ddl::object{
    {"name", "Alice"},
    {"skills", ddl::array{"C++", "Python"}}
};

std::cout << data << '\n';
```

Possible output:

```text
{
  "skills": [
    C++,
    Python
  ],
  "name": Alice
}
```

## Formatting

- Arrays and objects are formatted across multiple lines.
- Indentation uses two spaces per nesting level.
- Object keys are enclosed in double quotes.
- String values are printed without surrounding quotes.
- Empty arrays and objects are printed as `[]` and `{}`.
- Commas separate elements and properties, except after the last entry.

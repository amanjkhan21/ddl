# Arrays and Objects

DDL provides two container types for organizing dynamic data:

- `ddl::array` — a collection of values.
- `ddl::object` — a collection of key-value pairs.

Both containers can store different types of values and support nested structures.

## Arrays

### Creating an Array

`ddl::array` is an alias for `std::vector<ddl::value>`.

This means a DDL array can contain values of different types.

```cpp
ddl::array values = {
    42,
    "Hello",
    true,
    3.14,
    nullptr
};
```

Each element is a `ddl::value`, so you can store integers, strings, booleans, floating-point numbers, null values, and even other arrays or objects.

### Accessing Elements

Since `ddl::array` is a `std::vector<ddl::value>`, you can use familiar vector operations.

```cpp
ddl::array values = {10, 20, 30};

std::cout << values.size() << '\n';
std::cout << values[0] << '\n';
```

You can also add elements:

```cpp
values.push_back("Hello");
values.emplace_back(true);
```

### Nesting Arrays

Arrays can contain other arrays, allowing you to build nested structures.

```cpp
ddl::array values = {
    10,
    "Hello",
    ddl::array{20, 30, 40}
};
```

Nested arrays are also stored as `ddl::value` instances.

## Objects

### Creating an Object

`ddl::object` is an alias for `std::unordered_map<std::string, ddl::value>`.

It stores key-value pairs, where each key is a string and each value is a `ddl::value`.

```cpp
ddl::object person = {
    {"name", "Alice"},
    {"age", 20},
    {"isStudent", true},
    {"height", 1.75}
};
```

Each property can store a different type of value.

### Accessing Properties

Since `ddl::object` is a `std::unordered_map`, you can use familiar map operations.

```cpp id="u8ks9r"
std::cout << person["name"] << '\n';
std::cout << person["age"] << '\n';
```

You can also add or update properties:

```cpp id="8s4q3z"
person["city"] = "London";
person["age"] = 21;
```

### Nesting Objects and Arrays

Objects can contain arrays, other objects, or any other supported DDL value.

```cpp id="f4j7x2"
ddl::object person = {
    {"name", "Alice"},
    {"age", 20},
    {"hobbies", ddl::array{"Reading", "Coding"}},
    {"address", ddl::object{
        {"city", "London"},
        {"country", "UK"}
    }}
};
```

This allows you to represent complex, structured data using familiar C++ containers.

## Retrieving Arrays and Objects

You can retrieve a stored array or object using `as<T>()`.

Because `ddl::array` and `ddl::object` are aliases for standard C++ containers, you can also use their underlying types directly.

### Retrieving an Array

```cpp
ddl::value data = ddl::array{10, 20, 30};

std::vector<ddl::value> values =
    data.as<std::vector<ddl::value>>();

values.push_back(40);

std::cout << values[0] << '\n';
std::cout << values.size() << '\n';
```

### Retrieving an Object

```cpp
ddl::value data = ddl::object{
    {"name", "Alice"},
    {"age", 20}
};

std::unordered_map<std::string, ddl::value> person =
    data.as<std::unordered_map<std::string, ddl::value>>();

person["city"] = "London";

std::cout << person["name"] << '\n';
std::cout << person["city"] << '\n';
```

## Copy Behavior

Retrieving an array or object returns a copy of the stored container, not a reference to the original.

This means modifications to the retrieved container do not affect the original `ddl::value`.

```cpp
ddl::value original = ddl::array{10, 20, 30};

auto values = original.as<ddl::array>();

values.push_back(40);

std::cout << original << '\n';
std::cout << values << '\n';
```

Output:

```text
[
  10,
  20,
  30
]
[
  10,
  20,
  30,
  40
]
```

The original value remains unchanged.

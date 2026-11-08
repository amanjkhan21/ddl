# Arrays and Objects

DDL provides two collection types: `ddl::array` and `ddl::object`. Both store `ddl::value` elements, allowing collections to contain different supported types and nested data structures.

## Arrays

`ddl::array` is an alias for `std::vector<ddl::value>`.

### Creating an Array

You can initialize an array with values of different types.

```cpp
ddl::array values = {
    42,
    "Hello",
    true,
    nullptr,
    3.14
};
```

You can also store an array inside a `ddl::value`.

```cpp
ddl::value data = ddl::array{1, 2, 3};
```

### Accessing Elements

Since `ddl::array` uses `std::vector`, you can use standard vector operations to access its elements.

```cpp
ddl::array values = {10, 20, 30};

std::cout << values[0].as<int>() << '\n'; // 10
std::cout << values.at(1).as<int>() << '\n'; // 20
```

### Modifying Elements

You can modify elements by assigning new values.

```cpp
ddl::array values = {10, 20, 30};

values[0] = 100;
values.push_back("New element");
```

### Iterating Over an Array

Use a range-based for loop to iterate over array elements.

```cpp
ddl::array values = {10, "Hello", true};

for (const ddl::value& item : values) {
    std::cout << item << '\n';
}
```

## Objects

`ddl::object` is an alias for `std::unordered_map<std::string, ddl::value>`.

Each object entry consists of a string key and a corresponding `ddl::value`.

### Creating an Object

Initialize an object with key-value pairs.

```cpp
ddl::object person = {
    {"name", "Alice"},
    {"age", 25},
    {"active", true}
};
```

You can also store an object inside a `ddl::value`.

```cpp
ddl::value data = ddl::object{
    {"name", "Alice"},
    {"age", 25}
};
```

### Accessing Elements

Use `operator[]` or `at()` to access entries by key.

```cpp
ddl::object person = {
    {"name", "Alice"},
    {"age", 25}
};

std::string name = person.at("name").as<std::string>();
int age = person["age"].as<int>();
```

`at()` throws `std::out_of_range` if the key does not exist. For a non-const object, `operator[]` inserts a default-constructed value if the key is missing.

### Modifying Elements

Assign a new value to an existing key or use `operator[]` to insert a new entry.

```cpp
ddl::object person = {
    {"name", "Alice"}
};

person["age"] = 25;
person["active"] = true;
person["name"] = "Bob";
```

### Iterating Over an Object

Use a range-based for loop to iterate over key-value pairs.

```cpp
ddl::object person = {
    {"name", "Alice"},
    {"age", 25}
};

for (const auto& [key, value] : person) {
    std::cout << key << ": " << value << '\n';
}
```

Object iteration order is unspecified because `ddl::object` uses `std::unordered_map`.

## Nested Collections

Arrays and objects can contain each other, allowing you to represent structured data.

```cpp
ddl::object person = {
    {"name", "Alice"},
    {"age", 25},
    {"skills", ddl::array{"C++", "Python"}},
    {"address", ddl::object{
        {"city", "London"},
        {"country", "UK"}
    }}
};

ddl::value data = person;
```

You can access nested values by combining collection access with `as<T>()`.

```cpp
const ddl::object& personData = data.as<ddl::object>();

std::string name =
    personData.at("name").as<std::string>();

std::string city =
    personData.at("address")
              .as<ddl::object>()
              .at("city")
              .as<std::string>();
```

## Working with Collections Through References

Use `ref<T>()` when you want to modify an array or object stored inside a `ddl::value` directly.

```cpp
ddl::value data = ddl::array{1, 2, 3};

ddl::array& values = data.ref<ddl::array>();

values.push_back(4);
```

For objects:

```cpp
ddl::value data = ddl::object{
    {"name", "Alice"}
};

ddl::object& person = data.ref<ddl::object>();

person["age"] = 25;
```

## Printing Collections

DDL supports printing arrays and objects using the stream insertion operator.

```cpp
ddl::array values = {1, "Hello", true};

ddl::object person = {
    {"name", "Alice"},
    {"active", true}
};

std::cout << values << '\n';
std::cout << person << '\n';
```

Object key order is unspecified, so the printed order of entries may vary.

Continue to [Printing](./printing) to learn how DDL formats values and nested collections.

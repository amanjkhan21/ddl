# Type Conversions

DDL provides `as<T>()` to retrieve a stored value as a requested type.

Conversions are explicit and follow defined rules. If a conversion is unsupported or the value cannot be represented by the requested type, DDL throws an exception.

## Basic Usage

```cpp
ddl::value data = 42;

int number = data.as<int>();
double decimal = data.as<double>();
std::string text = data.as<std::string>();
```

Each call requests a conversion to a particular type.

## Conversion Rules

The following table summarizes the supported conversions.

| Stored Type    | bool | Integers | float / double | char | string |
| -------------- | ---- | -------- | -------------- | ---- | ------ |
| null           | Yes  | Yes      | Yes            | No   | Yes    |
| bool           | Yes  | Yes      | Yes            | No   | Yes    |
| Integers       | Yes  | Yes      | Yes            | No   | Yes    |
| float / double | Yes  | Yes      | Yes            | No   | Yes    |
| char           | Yes  | Yes      | Yes            | Yes  | Yes    |
| string         | Yes  | No       | No             | No   | Yes    |
| array          | No   | No       | No             | No   | No     |
| object         | No   | No       | No             | No   | No     |

A `Yes` indicates that the conversion is supported. Some conversions may still throw exceptions when the value is outside the target type's range.

Arrays and objects can only be retrieved as their exact types.

## Boolean Conversions

DDL uses truthiness when converting values to `bool`.

- Null becomes `false`.
- Boolean values remain unchanged.
- Numeric zero becomes `false`; nonzero becomes `true`.
- The null character (`'\0'`) becomes `false`.
- Empty strings become `false`; nonempty strings become `true`.
- Arrays and objects cannot be converted to `bool`.

```cpp
ddl::value a = 0;
ddl::value b = "Hello";

bool first = a.as<bool>();  // false
bool second = b.as<bool>(); // true
```

## Integer Conversions

Conversions to `int32_t` and `int64_t` support null, booleans, numeric values, and characters.

Floating-point values are truncated toward zero.

```cpp
ddl::value data = 12.8;

int number = data.as<int>(); // 12
```

DDL checks floating-point values for finiteness and range before converting them. Invalid or out-of-range conversions throw `std::out_of_range`.

## Floating-Point Conversions

Values stored as null, booleans, integers, floating-point numbers, and characters can be converted to `float` or `double`.

When converting a finite `double` to `float`, an overflow throws `std::out_of_range`.

NaN and infinity are preserved where supported by the target floating-point type.

## Character Conversions

Character conversion is intentionally strict.

Only a value already storing a `char` can be retrieved as `char`.

```cpp
ddl::value data = 'A';

char character = data.as<char>();
```

An integer value such as `65` cannot be retrieved as a character, even though it has the same numeric code.

## String Conversions

DDL supports converting scalar values to strings.

Examples:

```cpp
ddl::value a = 42;
ddl::value b = true;
ddl::value c = 'A';
ddl::value d = nullptr;

std::string first = a.as<std::string>();  // "42"
std::string second = b.as<std::string>(); // "true"
std::string third = c.as<std::string>();  // "A"
std::string fourth = d.as<std::string>(); // "null"
```

Arrays and objects cannot be converted to strings using `as<std::string>()`.

## Arrays and Objects

DDL provides two container types:

- `ddl::array`, an alias for `std::vector<ddl::value>`.
- `ddl::object`, an alias for `std::unordered_map<std::string, ddl::value>`.

Both containers can hold different value types and support nested structures.

### Arrays

You can retrieve a DDL array as a regular `std::vector<ddl::value>` using `as<ddl::array>()`.

```cpp
ddl::value data = ddl::array{42, "Hello", true};

std::vector<ddl::value> values = data.as<ddl::array>();

// Work with it like a regular vector
values.push_back(3.14);

std::cout << values.size() << '\n'; // 4
std::cout << values[0].as<int>() << '\n'; // 42
```

### Objects

Similarly, you can retrieve a DDL object as a regular `std::unordered_map<std::string, ddl::value>`.

```cpp
ddl::value data = ddl::object{
    {"name", "Alice"},
    {"age", 20},
    {"active", true}
};

std::unordered_map<std::string, ddl::value> properties = data.as<ddl::object>();

// Work with it like a regular unordered_map
properties["city"] = "London";

std::cout << properties["age"].as<std::int32_t>() << '\n'; // 20
std::cout << properties["city"].as<std::string>() << '\n'; // London
```

### Working with Copies

Both conversions return copies of the stored containers.

This means you can modify the retrieved vector or unordered map without changing the original `ddl::value`.

```cpp
ddl::value original = ddl::array{10, 20, 30};

std::vector<ddl::value> values = original.as<ddl::array>();
values.push_back(40);

// original still contains only 10, 20, and 30
```

## Exceptions

DDL uses standard C++ exceptions to report conversion failures.

| Exception               | When it occurs                                                                   |
| ----------------------- | -------------------------------------------------------------------------------- |
| `std::invalid_argument` | The requested conversion is unsupported                                          |
| `std::out_of_range`     | A numeric conversion is outside the target type's range or cannot be represented |

Unsupported target types are rejected at compile time.

## Summary

- Use `as<T>()` to retrieve or convert a value.
- Conversions follow explicit rules.
- Numeric conversions include range checks.
- Arrays and objects require exact type matches and return copies.
- Unsupported conversions throw exceptions.

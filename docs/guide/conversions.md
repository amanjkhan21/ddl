# Conversions

DDL provides the `as<T>()` method to retrieve a stored value as a specified C++ type.

Unlike `is<T>()`, which checks the exact stored type, `as<T>()` can convert between supported types.

## Using `as<T>()`

Call `as<T>()` with the desired destination type.

```cpp
ddl::value data = 42;

int number = data.as<int>();
double decimal = data.as<double>();

std::cout << number << '\n';  // 42
std::cout << decimal << '\n'; // 42
```

The original stored value is not changed by a conversion.

## Boolean Conversions

Values can be converted to `bool`.

| Source Type      | Result                              |
| ---------------- | ----------------------------------- |
| `std::nullptr_t` | `false`                             |
| `bool`           | Original value                      |
| Numeric types    | `false` if zero, otherwise `true`   |
| `char`           | `false` if `'\0'`, otherwise `true` |
| `std::string`    | `false` if empty, otherwise `true`  |
| `ddl::array`     | `true`                              |
| `ddl::object`    | `true`                              |

```cpp
ddl::value data = 42;

bool result = data.as<bool>(); // true
```

## Numeric Conversions

DDL supports conversions between numeric types:

- `bool`
- `float`
- `double`
- `std::int32_t`
- `std::int64_t`

Null values can also be converted to numeric types and produce zero.

```cpp
ddl::value integer = 42;

float single = integer.as<float>();
double decimal = integer.as<double>();
std::int64_t large = integer.as<std::int64_t>();
```

Floating-point values converted to integer types use C++ numeric conversion behavior, discarding the fractional part.

Conversions to `float`, `std::int32_t`, and `std::int64_t` perform range checks where applicable. A value outside the destination type's representable range throws `std::out_of_range`.

## String Conversions

The following types can be converted to `std::string`:

- `std::nullptr_t`
- `bool`
- `float`
- `double`
- `char`
- `std::int32_t`
- `std::int64_t`
- `std::string`

```cpp
ddl::value data = 42;

std::string text = data.as<std::string>();

std::cout << text << '\n'; // 42
```

Arrays and objects cannot be converted to strings using `as<std::string>()`.

## Exact-Type Conversions

Some types can only be retrieved when the stored type matches exactly.

| Destination Type | Supported Source                   |
| ---------------- | ---------------------------------- |
| `char`           | `char`                             |
| `std::string`    | Supported string-convertible types |
| `ddl::array`     | `ddl::array`                       |
| `ddl::object`    | `ddl::object`                      |
| `std::nullptr_t` | `std::nullptr_t`                   |

For example:

```cpp
ddl::value data = ddl::array{1, 2, 3};

ddl::array values = data.as<ddl::array>();
```

## Conversion Errors

DDL uses exceptions to report unsupported conversions and numeric range errors.

### `std::invalid_argument`

Thrown when the requested conversion is not supported.

```cpp
ddl::value data = "Hello";

try {
    int number = data.as<int>();
} catch (const std::invalid_argument& error) {
    std::cerr << error.what() << '\n';
}
```

### `std::out_of_range`

Thrown when a numeric value exceeds the representable range of the requested destination type.

```cpp
ddl::value data = 1e100;

try {
    float number = data.as<float>();
} catch (const std::out_of_range& error) {
    std::cerr << error.what() << '\n';
}
```

## Important Notes

- `as<T>()` returns a value, not a reference.
- Conversions do not modify the original stored value.
- Unsupported conversions throw `std::invalid_argument`.
- Numeric range violations throw `std::out_of_range`.
- Floating-point to integer conversions discard the fractional part.

Continue to [References](./references) to learn how to access and modify stored values without copying them.

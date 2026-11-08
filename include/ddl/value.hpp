#pragma once

// DDL version
#define DDL_VERSION_MAJOR 2
#define DDL_VERSION_MINOR 0
#define DDL_VERSION_PATCH 0

// Standard library headers
#include <cstddef>
#include <cstdint>
#include <ostream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <variant>
#include <vector>

namespace ddl {

// Forward declaration
class value;

// Container type aliases
using array = std::vector<value>;
using object = std::unordered_map<std::string, value>;

// Supported value types
template <typename T>
inline constexpr bool is_supported_type = (
    std::is_same_v<T, std::nullptr_t> ||
    std::is_same_v<T, bool> ||
    std::is_same_v<T, std::int32_t> ||
    std::is_same_v<T, std::int64_t> ||
    std::is_same_v<T, float> ||
    std::is_same_v<T, double> ||
    std::is_same_v<T, char> ||
    std::is_same_v<T, std::string> ||
    std::is_same_v<T, array> ||
    std::is_same_v<T, object>
);

/**
 * @brief Represents a dynamically typed value.
 *
 * A value can store any of the types supported by DDL, including
 * primitive values, strings, arrays, and objects.
 */
class value {
private:
    // Internal storage
    using type = std::variant<
        std::nullptr_t,
        bool,
        float,
        double,
        char,
        std::int32_t,
        std::int64_t,
        std::string,
        array,
        object
    >;

    type data;

    // Internal conversion functions
    bool to_bool() const;
    float to_float() const;
    double to_double() const;
    std::int32_t to_int32() const;
    std::int64_t to_int64() const;
    std::string to_string() const;

public:
    // Constructors
    value() = default;
    value(std::nullptr_t);
    value(bool v);
    value(float v);
    value(double v);
    value(char v);
    value(std::int32_t v);
    value(std::int64_t v);
    value(std::string v);
    value(array v);
    value(object v);
    value(const char* v);

    /**
     * @brief Checks whether the stored value has the specified type.
     *
     * @tparam T The type to check.
     * @return true if the stored value has type T, otherwise false.
     */
    template <typename T>
    bool is() const {
        return std::holds_alternative<T>(data);
    }

    /**
     * @brief Converts the stored value to the specified type.
     *
     * @tparam T The requested type.
     * @return The converted value.
     * @throws std::invalid_argument If the conversion is unsupported.
     */
    template <typename T>
    T as() const {
        if constexpr (std::is_same_v<T, std::nullptr_t>) {
            if (is<std::nullptr_t>()) {
                return nullptr;
            }

            throw std::invalid_argument("Unsupported conversion to 'nullptr'");
        } else if constexpr (std::is_same_v<T, bool>) {
            return to_bool();
        } else if constexpr (std::is_same_v<T, float>) {
            return to_float();
        } else if constexpr (std::is_same_v<T, double>) {
            return to_double();
        } else if constexpr (std::is_same_v<T, char>) {
            if (is<char>()) {
                return std::get<char>(data);
            }

            throw std::invalid_argument("Unsupported conversion to 'char'");
        } else if constexpr (std::is_same_v<T, std::int32_t>) {
            return to_int32();
        } else if constexpr (std::is_same_v<T, std::int64_t>) {
            return to_int64();
        } else if constexpr (std::is_same_v<T, std::string>) {
            return to_string();
        } else if constexpr (std::is_same_v<T, array>) {
            if (is<array>()) {
                return std::get<array>(data);
            }

            throw std::invalid_argument("Unsupported conversion to 'array'");
        } else if constexpr (std::is_same_v<T, object>) {
            if (is<object>()) {
                return std::get<object>(data);
            }

            throw std::invalid_argument("Unsupported conversion to 'object'");
        } else {
            static_assert(
                is_supported_type<T>,
                "Unsupported type requested from ddl::value::as<T>()"
            );
        }
    }

    /**
     * @brief Returns a mutable reference to the stored value.
     *
     * The requested type must exactly match the stored type.
     * Use ddl::value as T to obtain a reference to the entire value.
     *
     * @tparam T The requested reference type.
     * @return A mutable reference to the stored value.
     * @throws std::invalid_argument If the stored type does not match T.
     */
    template <typename T>
    T& ref() & {
        if constexpr (std::is_same_v<T, value>) {
            return *this;
        } else if constexpr (is_supported_type<T>) {
            if (auto* ptr = std::get_if<T>(&data)) {
                return *ptr;
            }

            throw std::invalid_argument(
                "Stored type does not match requested reference type"
            );
        } else {
            static_assert(
                is_supported_type<T>,
                "Unsupported type requested from ddl::value::ref<T>()"
            );
        }
    }

    /**
     * @brief Returns a constant reference to the stored value.
     *
     * The requested type must exactly match the stored type.
     * Use ddl::value as T to obtain a reference to the entire value.
     *
     * @tparam T The requested reference type.
     * @return A constant reference to the stored value.
     * @throws std::invalid_argument If the stored type does not match T.
     */
    template <typename T>
    const T& ref() const & {
        if constexpr (std::is_same_v<T, value>) {
            return *this;
        } else if constexpr (is_supported_type<T>) {
            if (auto* ptr = std::get_if<T>(&data)) {
                return *ptr;
            }

            throw std::invalid_argument(
                "Stored type does not match requested reference type"
            );
        } else {
            static_assert(
                is_supported_type<T>,
                "Unsupported type requested from ddl::value::ref<T>()"
            );
        }
    }

    // Prevent references to temporary values
    template <typename T>
    T& ref() && = delete;

    template <typename T>
    const T& ref() const && = delete;
};

/**
 * @brief Writes a value to an output stream.
 *
 * @param os The output stream.
 * @param v The value to write.
 * @return A reference to the output stream.
 */
std::ostream& operator<<(std::ostream& os, const value& v);

} // namespace ddl
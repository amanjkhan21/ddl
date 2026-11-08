#pragma once

// DDL version
#define DDL_VERSION_MAJOR 2
#define DDL_VERSION_MINOR 0
#define DDL_VERSION_PATCH 0

// Standard library headers
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <limits>
#include <ostream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>
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

public:
    // Constructors
    value() = default;
    value(std::nullptr_t) : data(nullptr) {}
    value(bool v) : data(v) {}
    value(float v) : data(v) {}
    value(double v) : data(v) {}
    value(char v) : data(v) {}
    value(std::int32_t v) : data(v) {}
    value(std::int64_t v) : data(v) {}
    value(std::string v) : data(std::move(v)) {}
    value(array v) : data(std::move(v)) {}
    value(object v) : data(std::move(v)) {}
    value(const char* v) : data(v ? std::string(v) : type(nullptr)) {}

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
            if (is<std::nullptr_t>()) {
                return false;
            } else if (is<bool>()) {
                return ref<bool>();
            } else if (is<std::int32_t>()) {
                return ref<std::int32_t>() != 0;
            } else if (is<std::int64_t>()) {
                return ref<std::int64_t>() != 0;
            } else if (is<float>()) {
                return ref<float>() != 0.0f;
            } else if (is<double>()) {
                return ref<double>() != 0.0;
            } else if (is<char>()) {
                return ref<char>() != '\0';
            } else if (is<std::string>()) {
                return !ref<std::string>().empty();
            } else if (is<array>() || is<object>()) {
                return true;
            }

            throw std::invalid_argument("Unsupported conversion to 'bool'");
        } else if constexpr (std::is_same_v<T, float>) {
            if (is<std::nullptr_t>()) {
                return 0.0f;
            } else if (is<bool>()) {
                return ref<bool>() ? 1.0f : 0.0f;
            } else if (is<std::int32_t>()) {
                return static_cast<float>(ref<std::int32_t>());
            } else if (is<std::int64_t>()) {
                return static_cast<float>(ref<std::int64_t>());
            } else if (is<float>()) {
                return ref<float>();
            } else if (is<double>()) {
                double v = ref<double>();

                if (std::isfinite(v) &&
                    (v > std::numeric_limits<float>::max() ||
                    v < -std::numeric_limits<float>::max())) {
                    throw std::out_of_range("Cannot convert 'double' to 'float'");
                }

                return static_cast<float>(v);
            }

            throw std::invalid_argument("Unsupported conversion to 'float'");
        } else if constexpr (std::is_same_v<T, double>) {
            if (is<std::nullptr_t>()) {
                return 0.0;
            } else if (is<bool>()) {
                return ref<bool>() ? 1.0 : 0.0;
            } else if (is<std::int32_t>()) {
                return static_cast<double>(ref<std::int32_t>());
            } else if (is<std::int64_t>()) {
                return static_cast<double>(ref<std::int64_t>());
            } else if (is<float>()) {
                return static_cast<double>(ref<float>());
            } else if (is<double>()) {
                return ref<double>();
            }

            throw std::invalid_argument("Unsupported conversion to 'double'");
        } else if constexpr (std::is_same_v<T, char>) {
            if (is<char>()) {
                return std::get<char>(data);
            }

            throw std::invalid_argument("Unsupported conversion to 'char'");
        } else if constexpr (std::is_same_v<T, std::int32_t>) {
            if (is<std::nullptr_t>()) {
                return 0;
            } else if (is<bool>()) {
                return ref<bool>() ? 1 : 0;
            } else if (is<std::int32_t>()) {
                return ref<std::int32_t>();
            } else if (is<std::int64_t>()) {
                std::int64_t v = ref<std::int64_t>();

                if (v < std::numeric_limits<std::int32_t>::min() ||
                    v > std::numeric_limits<std::int32_t>::max()) {
                    throw std::out_of_range("'int32_t' overflow");
                }

                return static_cast<std::int32_t>(v);
            } else if (is<float>()) {
                float v = ref<float>();

                if (!std::isfinite(v) ||
                    static_cast<double>(v) <= -2147483649.0 ||
                    static_cast<double>(v) >= 2147483648.0) {
                    throw std::out_of_range("'int32_t' overflow");
                }

                return static_cast<std::int32_t>(v);
            } else if (is<double>()) {
                double v = ref<double>();

                if (!std::isfinite(v) ||
                    v <= -2147483649.0 ||
                    v >= 2147483648.0) {
                    throw std::out_of_range("'int32_t' overflow");
                }

                return static_cast<std::int32_t>(v);
            }

            throw std::invalid_argument("Unsupported conversion to 'int32_t'");
        } else if constexpr (std::is_same_v<T, std::int64_t>) {
            if (is<std::nullptr_t>()) {
                return 0;
            } else if (is<bool>()) {
                return ref<bool>() ? 1 : 0;
            } else if (is<std::int32_t>()) {
                return static_cast<std::int64_t>(ref<std::int32_t>());
            } else if (is<std::int64_t>()) {
                return ref<std::int64_t>();
            } else if (is<float>()) {
                float v = ref<float>();

                if (!std::isfinite(v) ||
                    v < -9223372036854775808.0 ||
                    v >= 9223372036854775808.0) {
                    throw std::out_of_range("'int64_t' overflow");
                }

                return static_cast<std::int64_t>(v);
            } else if (is<double>()) {
                double v = ref<double>();

                if (!std::isfinite(v) ||
                    v < -9223372036854775808.0 ||
                    v >= 9223372036854775808.0) {
                    throw std::out_of_range("'int64_t' overflow");
                }

                return static_cast<std::int64_t>(v);
            }

            throw std::invalid_argument("Unsupported conversion to 'int64_t'");
        } else if constexpr (std::is_same_v<T, std::string>) {
            if (is<std::nullptr_t>()) {
                return "null";
            } else if (is<bool>()) {
                return as<bool>() ? "true" : "false";
            } else if (is<std::int32_t>()) {
                return std::to_string(std::get<std::int32_t>(data));
            } else if (is<std::int64_t>()) {
                return std::to_string(std::get<std::int64_t>(data));
            } else if (is<float>()) {
                return std::to_string(std::get<float>(data));
            } else if (is<double>()) {
                return std::to_string(std::get<double>(data));
            } else if (is<char>()) {
                return std::string(1, std::get<char>(data));
            } else if (is<std::string>()) {
                return std::get<std::string>(data);
            }

            throw std::invalid_argument("Unsupported conversion to 'string'");
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

// Recursive value printer
inline void print(std::ostream& os, const ddl::value& v, std::size_t indent) {
    const std::string padding(indent, ' ');
    const std::string child_padding(indent + 2, ' ');

    if (v.is<std::nullptr_t>()) {
        os << "null";
    } else if (v.is<bool>()) {
        os << (v.as<bool>() ? "true" : "false");
    } else if (v.is<std::int32_t>()) {
        os << v.as<std::int32_t>();
    } else if (v.is<std::int64_t>()) {
        os << v.as<std::int64_t>();
    } else if (v.is<float>()) {
        os << v.as<float>();
    } else if (v.is<double>()) {
        os << v.as<double>();
    } else if (v.is<char>()) {
        os << '\'' << v.as<char>() << '\'';
    } else if (v.is<std::string>()) {
        os << '"' << v.as<std::string>() << '"';
    } else if (v.is<ddl::array>()) {
        const auto& items = v.ref<ddl::array>();

        if (items.empty()) {
            os << "[]";
            return;
        }

        os << "[\n";

        for (std::size_t i = 0; i < items.size(); ++i) {
            os << child_padding;
            print(os, items[i], indent + 2);

            if (i != items.size() - 1) {
                os << ',';
            }

            os << '\n';
        }

        os << padding << ']';
    } else if (v.is<ddl::object>()) {
        const auto& items = v.ref<ddl::object>();

        if (items.empty()) {
            os << "{}";
            return;
        }

        os << "{\n";

        std::size_t i = 0;

        for (const auto& [key, item] : items) {
            os << child_padding << '"' << key << "\": ";
            print(os, item, indent + 2);

            if (i != items.size() - 1) {
                os << ',';
            }

            os << '\n';
            ++i;
        }

        os << padding << '}';
    }
}

/**
 * @brief Writes a value to an output stream.
 *
 * @param os The output stream.
 * @param v The value to write.
 * @return A reference to the output stream.
 */
inline std::ostream& operator<<(std::ostream& os, const value& v) {
    print(os, v, 0);
    return os;
}

} // namespace ddl
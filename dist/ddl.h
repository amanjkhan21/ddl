#pragma once

#define DDL_VERSION_MAJOR 1
#define DDL_VERSION_MINOR 0
#define DDL_VERSION_PATCH 0

#include <vector>
#include <unordered_map>
#include <variant>
#include <cstddef>
#include <cstdint>
#include <string>
#include <stdexcept>
#include <type_traits>
#include <ostream>
#include <utility>
#include <limits>
#include <cmath>

namespace ddl {
    class value;

    using array = std::vector<value>;
    using object = std::unordered_map<std::string, value>;
    using type = std::variant<
                    std::nullptr_t,
                    bool,
                    std::int32_t,
                    std::int64_t,
                    float,
                    double,
                    char,
                    std::string,
                    array,
                    object
                 >;

    /* ==================== Class ==================== */
    class value {
        private:
            type data;

            /* ==================== Helper Methods ==================== */
            bool to_bool() const;
            std::int32_t to_int32() const;
            std::int64_t to_int64() const;
            float to_float() const;
            double to_double() const;
            std::string to_string() const;
            
        public:
            /* ==================== Constructors ==================== */
            value() = default;
            value(std::nullptr_t); 
            value(bool v);
            value(std::int32_t v); 
            value(std::int64_t v); 
            value(float v); 
            value(double v); 
            value(char v); 
            value(std::string v); 
            value(const char* v); 
            value(array v);
            value(object v);

            /* ==================== Type Checker ==================== */
            template<typename T>
            bool is() const {
                return std::holds_alternative<T>(data);
            }

            /* ==================== Value Getter ==================== */
            template<typename T>
            T as() const {
                if constexpr (std::is_same_v<T, std::nullptr_t>) {
                    if (is<std::nullptr_t>()) {
                        return nullptr;
                    }

                    throw std::invalid_argument("Unsupported conversion to nullptr");
                } else if constexpr (std::is_same_v<T, bool>) {
                    return to_bool();
                } else if constexpr (std::is_same_v<T, std::int32_t>) {
                    return to_int32();   
                } else if constexpr (std::is_same_v<T, std::int64_t>) {
                    return to_int64();
                } else if constexpr (std::is_same_v<T, float>) {
                    return to_float();
                } else if constexpr (std::is_same_v<T, double>) {
                    return to_double();
                } else if constexpr (std::is_same_v<T, char>) {
                    if (is<char>()) {
                        return std::get<char>(data);
                    }

                    throw std::invalid_argument("Unsupported conversion to char");
                } else if constexpr (std::is_same_v<T, std::string>) {
                    return to_string();
                } else if constexpr (std::is_same_v<T, array>) {
                    if (is<array>()) {
                        return std::get<array>(data);
                    }

                    throw std::invalid_argument("Unsupported conversion to array");
                } else if constexpr (std::is_same_v<T, object>) {
                    if (is<object>()) {
                        return std::get<object>(data);
                    }

                    throw std::invalid_argument("Unsupported conversion to object");
                } else {
                    static_assert(
                        std::is_same_v<T, void>,
                        "Unsupported type requested from ddl::value::as<T>()"
                    );
                }
            }
    };

    /* ==================== Value Printer ==================== */
    std::ostream& operator<<(std::ostream& os, const value& v);
}

/* ==================== Helper Method Definitions ==================== */
inline bool ddl::value::to_bool() const {
    if (is<std::nullptr_t>()) {
        return false;
    } else if (is<bool>()) {
        return std::get<bool>(data);
    } else if (is<std::int32_t>()) {
        return as<std::int32_t>() != 0;
    } else if (is<std::int64_t>()) {
        return as<std::int64_t>() != 0;
    } else if (is<float>()) {
        return as<float>() != 0.0f;
    } else if (is<double>()) {
        return as<double>() != 0.0;
    } else if (is<char>()) {
        return as<char>() != '\0';
    } else if (is<std::string>()) {
        return !as<std::string>().empty();
    } else if (is<array>()) {
        throw std::invalid_argument("Unsupported conversion from array to bool");
    } else if (is<object>()) {
        throw std::invalid_argument("Unsupported conversion from object to bool");
    }

    throw std::invalid_argument("Unsupported conversion to bool");
}

inline std::int32_t ddl::value::to_int32() const {
    if (is<std::nullptr_t>()) {
        return 0;
    } else if (is<bool>()) {
        return (as<bool>() ? 1 : 0);
    } else if (is<std::int32_t>()) {
        return std::get<std::int32_t>(data);
    } else if (is<std::int64_t>()) {
        std::int64_t v = std::get<std::int64_t>(data);

        if (v < std::numeric_limits<std::int32_t>::min() ||
            v > std::numeric_limits<std::int32_t>::max()) {
            throw std::out_of_range("int64 value out of int32 range");
        }

        return static_cast<std::int32_t>(v);
    } else if (is<float>()) {
        float v = std::get<float>(data);

        if (!std::isfinite(v) ||
            static_cast<double>(v) <= -2147483649.0 ||
            static_cast<double>(v) >= 2147483648.0) {
            throw std::out_of_range("float value out of int32 range");
        }

        return static_cast<std::int32_t>(v);
    } else if (is<double>()) {
        double v = std::get<double>(data);

        if (!std::isfinite(v) ||
            v <= -2147483649.0 ||
            v >= 2147483648.0) {
            throw std::out_of_range("double value out of int32 range");
        }

        return static_cast<std::int32_t>(v);
    } else if (is<char>()) {
        return static_cast<std::int32_t>(std::get<char>(data));
    } else if (is<std::string>()) {
        throw std::invalid_argument("Cannot convert string to int32");
    } else if (is<array>()) {
        throw std::invalid_argument("Unsupported conversion from array to int32");
    } else if (is<object>()) {
        throw std::invalid_argument("Unsupported conversion from object to int32");
    }

    throw std::invalid_argument("Unsupported conversion to int32");
}

inline std::int64_t ddl::value::to_int64() const {
    if (is<std::nullptr_t>()) {
        return 0;
    } else if (is<bool>()) {
        return (as<bool>() ? 1 : 0);
    } else if (is<std::int32_t>()) {
        return static_cast<std::int64_t>(std::get<std::int32_t>(data));
    } else if (is<std::int64_t>()) {
        return std::get<int64_t>(data);
    } else if (is<float>()) {
        float v = std::get<float>(data);

        if (!std::isfinite(v) ||
            v < -9223372036854775808.0 ||
            v >= 9223372036854775808.0) {
            throw std::out_of_range("float value out of int64 range");
        }

        return static_cast<std::int64_t>(v);
    } else if (is<double>()) {
        double v = std::get<double>(data);

        if (!std::isfinite(v) ||
            v < -9223372036854775808.0 ||
            v >= 9223372036854775808.0) {
            throw std::out_of_range("double value out of int64 range");
        }

        return static_cast<std::int64_t>(v);
    } else if (is<char>()) {
        return static_cast<std::int64_t>(std::get<char>(data));
    } else if (is<std::string>()) {
        throw std::invalid_argument("Cannot convert string to int64");
    } else if (is<array>()) {
        throw std::invalid_argument("Unsupported conversion from array to int64");
    } else if (is<object>()) {
        throw std::invalid_argument("Unsupported conversion from object to int64");
    }

    throw std::invalid_argument("Unsupported conversion to int64");
}

inline float ddl::value::to_float() const {
    if (is<std::nullptr_t>()) {
        return 0.0f;
    } else if (is<bool>()) {
        return as<bool>() ? 1.0f : 0.0f;
    } else if (is<std::int32_t>()) {
        return static_cast<float>(std::get<std::int32_t>(data));
    } else if (is<std::int64_t>()) {
        return static_cast<float>(std::get<std::int64_t>(data));
    } else if (is<float>()) {
        return std::get<float>(data);
    } else if (is<double>()) {
        double v = std::get<double>(data);

        if (std::isfinite(v) &&
            (v > std::numeric_limits<float>::max() ||
             v < -std::numeric_limits<float>::max())) {
            throw std::out_of_range("Cannot convert double to float");
        }

        return static_cast<float>(v);
    } else if (is<char>()) {
        return static_cast<float>(std::get<char>(data));
    } else if (is<std::string>()) {
        throw std::invalid_argument("Cannot convert string to float");
    } else if (is<array>()) {
        throw std::invalid_argument("Unsupported conversion from array to float");
    } else if (is<object>()) {
        throw std::invalid_argument("Unsupported conversion from object to float");
    }

    throw std::invalid_argument("Unsupported conversion to float");
}

inline double ddl::value::to_double() const {
    if (is<std::nullptr_t>()) {
        return 0.0;
    } else if (is<bool>()) {
        return as<bool>() ? 1.0 : 0.0;
    } else if (is<std::int32_t>()) {
        return static_cast<double>(std::get<std::int32_t>(data));
    } else if (is<std::int64_t>()) {
        return static_cast<double>(std::get<std::int64_t>(data));
    } else if (is<float>()) {
        return static_cast<double>(std::get<float>(data));
    } else if (is<double>()) {
        return std::get<double>(data);
    } else if (is<char>()) {
        return static_cast<double>(std::get<char>(data));
    } else if (is<std::string>()) {
        throw std::invalid_argument("Cannot convert string to double");
    } else if (is<array>()) {
        throw std::invalid_argument("Unsupported conversion from array to double");
    } else if (is<object>()) {
        throw std::invalid_argument("Unsupported conversion from object to double");
    }

    throw std::invalid_argument("Unsupported conversion to double");
}

inline std::string ddl::value::to_string() const {
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
    } else if (is<array>()) {
        throw std::invalid_argument("Unsupported conversion from array to string");
    } else if (is<object>()) {
        throw std::invalid_argument("Unsupported conversion from object to string");
    }

    throw std::invalid_argument("Unsupported conversion to string");
}

/* ==================== Constructor Definitions ==================== */
inline ddl::value::value(std::nullptr_t) : data(nullptr) {}
inline ddl::value::value(bool v) : data(v) {}
inline ddl::value::value(std::int32_t v) : data(v) {}
inline ddl::value::value(std::int64_t v) : data(v) {}
inline ddl::value::value(float v) : data(v) {}
inline ddl::value::value(double v) : data(v) {}
inline ddl::value::value(char v) : data(v) {}
inline ddl::value::value(std::string v) : data(std::move(v)) {}
inline ddl::value::value(const char* v) : data(v ? std::string(v) : type(nullptr)) {}
inline ddl::value::value(array v) : data(std::move(v)) {}
inline ddl::value::value(object v) : data(std::move(v)) {}

/* ====================  Print Helper ==================== */
namespace {
    void print(std::ostream& os, const ddl::value& v, std::size_t indent) {
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
            os << v.as<char>();
        } else if (v.is<std::string>())  {
            os << v.as<std::string>();
        } else if (v.is<ddl::array>()) {
            const auto items = v.as<ddl::array>();

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
            const auto items = v.as<ddl::object>();

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
}

/* ==================== Value Printer Definition ==================== */
inline std::ostream& ddl::operator<<(std::ostream& os, const value& v) {
    print(os, v, 0);
    return os;
}
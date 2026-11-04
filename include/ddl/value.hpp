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
#include <ddl/value.hpp>

#include <cmath>
#include <limits>
#include <utility>

namespace {

// Recursive value printer
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

} // namespace

// Constructors
ddl::value::value(std::nullptr_t) : data(nullptr) {}
ddl::value::value(bool v) : data(v) {}
ddl::value::value(float v) : data(v) {}
ddl::value::value(double v) : data(v) {}
ddl::value::value(char v) : data(v) {}
ddl::value::value(std::int32_t v) : data(v) {}
ddl::value::value(std::int64_t v) : data(v) {}
ddl::value::value(std::string v) : data(std::move(v)) {}
ddl::value::value(array v) : data(std::move(v)) {}
ddl::value::value(object v) : data(std::move(v)) {}
ddl::value::value(const char* v) : data(v ? std::string(v) : type(nullptr)) {}

// Boolean conversion
bool ddl::value::to_bool() const {
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
}

// Float conversion
float ddl::value::to_float() const {
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
}

// Double conversion
double ddl::value::to_double() const {
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
}

// int32_t conversion
std::int32_t ddl::value::to_int32() const {
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
}

// int64_t conversion
std::int64_t ddl::value::to_int64() const {
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
}

// string conversion
std::string ddl::value::to_string() const {
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
}

// Stream insertion operator
std::ostream& ddl::operator<<(std::ostream& os, const value& v) {
    print(os, v, 0);
    return os;
}
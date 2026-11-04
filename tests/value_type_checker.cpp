#include <cassert>
#include <cstdint>
#include <string>
#include <ddl/value.hpp>

void test_value_type_checker() {
    using namespace ddl;

    // nullptr
    value null_value;
    assert(null_value.is<std::nullptr_t>());
    assert(!null_value.is<bool>());
    assert(!null_value.is<std::string>());

    // bool
    value bool_value(true);
    assert(bool_value.is<bool>());
    assert(!bool_value.is<std::int32_t>());

    // int32
    value int32_value(std::int32_t{42});
    assert(int32_value.is<std::int32_t>());
    assert(!int32_value.is<std::int64_t>());

    // int64
    value int64_value(std::int64_t{42});
    assert(int64_value.is<std::int64_t>());
    assert(!int64_value.is<std::int32_t>());

    // float
    value float_value(3.14f);
    assert(float_value.is<float>());
    assert(!float_value.is<double>());

    // double
    value double_value(3.14);
    assert(double_value.is<double>());
    assert(!double_value.is<float>());

    // char
    value char_value('A');
    assert(char_value.is<char>());
    assert(!char_value.is<std::string>());

    // string
    value string_value("hello");
    assert(string_value.is<std::string>());
    assert(!string_value.is<char>());
}

#include <cassert>
#include <cstdint>
#include <string>

#include <ddl/value.hpp>

void test_value_type_checker() {
    using namespace ddl;

    // -------------------------------------------------------------------------
    // nullptr
    // -------------------------------------------------------------------------

    value null_value;

    assert(null_value.is<std::nullptr_t>());
    assert(!null_value.is<bool>());
    assert(!null_value.is<std::int32_t>());
    assert(!null_value.is<std::int64_t>());
    assert(!null_value.is<float>());
    assert(!null_value.is<double>());
    assert(!null_value.is<char>());
    assert(!null_value.is<std::string>());
    assert(!null_value.is<array>());
    assert(!null_value.is<object>());

    // -------------------------------------------------------------------------
    // bool
    // -------------------------------------------------------------------------

    value bool_value(true);

    assert(bool_value.is<bool>());
    assert(!bool_value.is<std::nullptr_t>());
    assert(!bool_value.is<std::int32_t>());
    assert(!bool_value.is<std::int64_t>());

    value false_value(false);

    assert(false_value.is<bool>());

    // -------------------------------------------------------------------------
    // int32_t
    // -------------------------------------------------------------------------

    value int32_value(std::int32_t{42});

    assert(int32_value.is<std::int32_t>());
    assert(!int32_value.is<std::int64_t>());
    assert(!int32_value.is<float>());
    assert(!int32_value.is<double>());

    // -------------------------------------------------------------------------
    // int64_t
    // -------------------------------------------------------------------------

    value int64_value(std::int64_t{42});

    assert(int64_value.is<std::int64_t>());
    assert(!int64_value.is<std::int32_t>());
    assert(!int64_value.is<float>());
    assert(!int64_value.is<double>());

    // -------------------------------------------------------------------------
    // float
    // -------------------------------------------------------------------------

    value float_value(3.14f);

    assert(float_value.is<float>());
    assert(!float_value.is<double>());
    assert(!float_value.is<std::int32_t>());
    assert(!float_value.is<std::int64_t>());

    // -------------------------------------------------------------------------
    // double
    // -------------------------------------------------------------------------

    value double_value(3.14);

    assert(double_value.is<double>());
    assert(!double_value.is<float>());
    assert(!double_value.is<std::int32_t>());
    assert(!double_value.is<std::int64_t>());

    // -------------------------------------------------------------------------
    // char
    // -------------------------------------------------------------------------

    value char_value('A');

    assert(char_value.is<char>());
    assert(!char_value.is<std::string>());
    assert(!char_value.is<std::int32_t>());

    // -------------------------------------------------------------------------
    // std::string
    // -------------------------------------------------------------------------

    value string_value("hello");

    assert(string_value.is<std::string>());
    assert(!string_value.is<char>());
    assert(!string_value.is<bool>());

    // Empty strings retain their string type
    value empty_string_value("");

    assert(empty_string_value.is<std::string>());

    // -------------------------------------------------------------------------
    // array
    // -------------------------------------------------------------------------

    value array_value(array{value(1), value("hello")});

    assert(array_value.is<array>());
    assert(!array_value.is<object>());
    assert(!array_value.is<std::string>());

    // Empty arrays retain their array type
    value empty_array_value(array{});

    assert(empty_array_value.is<array>());

    // -------------------------------------------------------------------------
    // object
    // -------------------------------------------------------------------------

    value object_value(object{
        {"name", value("DDL")},
        {"version", value(1)}
    });

    assert(object_value.is<object>());
    assert(!object_value.is<array>());
    assert(!object_value.is<std::string>());

    // Empty objects retain their object type
    value empty_object_value(object{});

    assert(empty_object_value.is<object>());
}
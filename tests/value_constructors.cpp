#include <cassert>
#include <cstdint>
#include <string>
#include <ddl/value.hpp>

void test_value_constructors() {
    using namespace ddl;

    // Default constructor
    value default_value;
    assert(default_value.is<std::nullptr_t>());

    // nullptr
    value null_value(nullptr);
    assert(null_value.is<std::nullptr_t>());

    // bool
    value bool_value(true);
    assert(bool_value.is<bool>());
    assert(bool_value.as<bool>() == true);

    // int32
    value int32_value(std::int32_t{42});
    assert(int32_value.is<std::int32_t>());
    assert(int32_value.as<std::int32_t>() == 42);

    // int64
    value int64_value(std::int64_t{123456789});
    assert(int64_value.is<std::int64_t>());
    assert(int64_value.as<std::int64_t>() == 123456789);

    // float
    value float_value(3.14f);
    assert(float_value.is<float>());
    assert(float_value.as<float>() == 3.14f);

    // double
    value double_value(3.14);
    assert(double_value.is<double>());
    assert(double_value.as<double>() == 3.14);

    // char
    value char_value('A');
    assert(char_value.is<char>());
    assert(char_value.as<char>() == 'A');

    // std::string
    value string_value(std::string("hello"));
    assert(string_value.is<std::string>());
    assert(string_value.as<std::string>() == "hello");

    // C-style string
    value cstring_value("ddlamic");
    assert(cstring_value.is<std::string>());
    assert(cstring_value.as<std::string>() == "ddlamic");

    // null c-style string
    const char* text = nullptr;
    ddl::value null_string(text);
    assert(null_string.is<std::nullptr_t>());

    // empty c-style string
    ddl::value empty_string("");
    assert(empty_string.is<std::string>());
    assert(empty_string.as<std::string>().empty());
}

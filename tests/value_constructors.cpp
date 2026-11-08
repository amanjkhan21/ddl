#include <cassert>
#include <cstdint>
#include <string>

#include <ddl/value.hpp>

void test_value_constructors() {
    using namespace ddl;

    // -------------------------------------------------------------------------
    // Default and null constructors
    // -------------------------------------------------------------------------

    value default_value;

    assert(default_value.is<std::nullptr_t>());
    assert(default_value.as<std::nullptr_t>() == nullptr);

    value null_value(nullptr);

    assert(null_value.is<std::nullptr_t>());
    assert(null_value.as<std::nullptr_t>() == nullptr);

    // -------------------------------------------------------------------------
    // Boolean constructor
    // -------------------------------------------------------------------------

    value bool_value(true);

    assert(bool_value.is<bool>());
    assert(bool_value.as<bool>() == true);

    value false_value(false);

    assert(false_value.is<bool>());
    assert(false_value.as<bool>() == false);

    // -------------------------------------------------------------------------
    // Integer constructors
    // -------------------------------------------------------------------------

    value int32_value(std::int32_t{42});

    assert(int32_value.is<std::int32_t>());
    assert(int32_value.as<std::int32_t>() == 42);

    value negative_int32_value(std::int32_t{-42});

    assert(negative_int32_value.is<std::int32_t>());
    assert(negative_int32_value.as<std::int32_t>() == -42);

    value int64_value(std::int64_t{123456789});

    assert(int64_value.is<std::int64_t>());
    assert(int64_value.as<std::int64_t>() == 123456789);

    value negative_int64_value(std::int64_t{-123456789});

    assert(negative_int64_value.is<std::int64_t>());
    assert(negative_int64_value.as<std::int64_t>() == -123456789);

    // -------------------------------------------------------------------------
    // Floating-point constructors
    // -------------------------------------------------------------------------

    value float_value(3.14f);

    assert(float_value.is<float>());
    assert(float_value.as<float>() == 3.14f);

    value double_value(3.14);

    assert(double_value.is<double>());
    assert(double_value.as<double>() == 3.14);

    // -------------------------------------------------------------------------
    // Character constructor
    // -------------------------------------------------------------------------

    value char_value('A');

    assert(char_value.is<char>());
    assert(char_value.as<char>() == 'A');

    value null_char_value('\0');

    assert(null_char_value.is<char>());
    assert(null_char_value.as<char>() == '\0');

    // -------------------------------------------------------------------------
    // String constructors
    // -------------------------------------------------------------------------

    value string_value(std::string("hello"));

    assert(string_value.is<std::string>());
    assert(string_value.as<std::string>() == "hello");

    value cstring_value("ddlamic");

    assert(cstring_value.is<std::string>());
    assert(cstring_value.as<std::string>() == "ddlamic");

    // Empty std::string
    value empty_string_value(std::string{});

    assert(empty_string_value.is<std::string>());
    assert(empty_string_value.as<std::string>().empty());

    // Empty C-style string
    value empty_cstring_value("");

    assert(empty_cstring_value.is<std::string>());
    assert(empty_cstring_value.as<std::string>().empty());

    // Null C-style string
    const char* null_text = nullptr;
    value null_cstring_value(null_text);

    assert(null_cstring_value.is<std::nullptr_t>());
    assert(null_cstring_value.as<std::nullptr_t>() == nullptr);
}
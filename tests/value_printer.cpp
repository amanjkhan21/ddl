#include <cassert>
#include <cstdint>
#include <sstream>
#include <string>

#include <ddl/value.hpp>

namespace {

void assert_prints(const ddl::value& input, const std::string& expected) {
    std::ostringstream output;
    output << input;

    assert(output.str() == expected);
}

} // namespace

void test_value_printer() {
    using namespace ddl;

    // -------------------------------------------------------------------------
    // Null
    // -------------------------------------------------------------------------

    assert_prints(value(nullptr), "null");

    // -------------------------------------------------------------------------
    // Boolean
    // -------------------------------------------------------------------------

    assert_prints(value(true), "true");
    assert_prints(value(false), "false");

    // -------------------------------------------------------------------------
    // Integers
    // -------------------------------------------------------------------------

    assert_prints(value(std::int32_t{42}), "42");
    assert_prints(value(std::int32_t{-42}), "-42");

    assert_prints(value(std::int64_t{123456789}), "123456789");
    assert_prints(value(std::int64_t{-123456789}), "-123456789");

    // -------------------------------------------------------------------------
    // Floating-point
    // -------------------------------------------------------------------------

    assert_prints(value(3.14f), "3.14");
    assert_prints(value(3.14), "3.14");

    assert_prints(value(0.0f), "0");
    assert_prints(value(0.0), "0");

    // -------------------------------------------------------------------------
    // Character
    // -------------------------------------------------------------------------

    assert_prints(value('A'), "'A'");
    assert_prints(value('0'), "'0'");

    // -------------------------------------------------------------------------
    // String
    // -------------------------------------------------------------------------

    assert_prints(value("hello"), "\"hello\"");
    assert_prints(value(std::string{}), "\"\"");

    // -------------------------------------------------------------------------
    // Empty containers
    // -------------------------------------------------------------------------

    assert_prints(value(array{}), "[]");
    assert_prints(value(object{}), "{}");

    // -------------------------------------------------------------------------
    // Arrays
    // -------------------------------------------------------------------------

    assert_prints(
        value(array{
            value(1),
            value(2),
            value(3)
        }),
        "[\n"
        "  1,\n"
        "  2,\n"
        "  3\n"
        "]"
    );

    // Mixed-type array
    assert_prints(
        value(array{
            value(42),
            value("hello"),
            value(true),
            value(nullptr)
        }),
        "[\n"
        "  42,\n"
        "  \"hello\",\n"
        "  true,\n"
        "  null\n"
        "]"
    );

    // -------------------------------------------------------------------------
    // Objects
    // -------------------------------------------------------------------------

    // Object property order is unspecified because ddl::object uses
    // std::unordered_map. Check the exact output only for a single property.
    assert_prints(
        value(object{
            {"name", value("DDL")}
        }),
        "{\n"
        "  \"name\": \"DDL\"\n"
        "}"
    );

    // -------------------------------------------------------------------------
    // Nested containers
    // -------------------------------------------------------------------------

    assert_prints(
        value(array{
            value(1),
            value(array{
                value(2),
                value(3)
            })
        }),
        "[\n"
        "  1,\n"
        "  [\n"
        "    2,\n"
        "    3\n"
        "  ]\n"
        "]"
    );

    assert_prints(
        value(object{
            {"items", value(array{
                value("first"),
                value("second")
            })}
        }),
        "{\n"
        "  \"items\": [\n"
        "    \"first\",\n"
        "    \"second\"\n"
        "  ]\n"
        "}"
    );
}
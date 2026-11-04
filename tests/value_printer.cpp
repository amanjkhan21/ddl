#include <cassert>
#include <sstream>
#include <string>
#include <ddl/value.hpp>

void test_value_printer() {
    using namespace ddl;

    std::ostringstream output;

    // nullptr
    output << value(nullptr);
    assert(output.str() == "null");

    // bool
    output.str("");
    output.clear();
    output << value(true);
    assert(output.str() == "true");

    output.str("");
    output.clear();
    output << value(false);
    assert(output.str() == "false");

    // Integers
    output.str("");
    output.clear();
    output << value(std::int32_t{42});
    assert(output.str() == "42");

    output.str("");
    output.clear();
    output << value(std::int64_t{123456789});
    assert(output.str() == "123456789");

    // Floating point
    output.str("");
    output.clear();
    output << value(3.14f);
    assert(output.str() == "3.14");

    output.str("");
    output.clear();
    output << value(3.14);
    assert(output.str() == "3.14");

    // char
    output.str("");
    output.clear();
    output << value('A');
    assert(output.str() == "A");

    // string
    output.str("");
    output.clear();
    output << value("hello");
    assert(output.str() == "hello");
}

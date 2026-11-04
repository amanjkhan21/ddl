#include <cassert>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include <ddl/value.hpp>

using ddl::value;
using ddl::array;

// ==================== Array Constructors ====================

void test_array_constructors() {
    value empty = array{};
    value numbers = array{1, 2, 3};
    value mixed = array{1, "Hello", 'a', false, 3.14};

    assert(empty.is<array>());
    assert(numbers.is<array>());
    assert(mixed.is<array>());

    assert(empty.as<array>().empty());
    assert(numbers.as<array>().size() == 3);
    assert(mixed.as<array>().size() == 5);
}

// ==================== Array Retrieval ====================

void test_array_retrieval() {
    value v = array{1, 2, 3};

    array items = v.as<array>();

    assert(items.size() == 3);
    assert(items[0].as<std::int32_t>() == 1);
    assert(items[1].as<std::int32_t>() == 2);
    assert(items[2].as<std::int32_t>() == 3);

    // Verify the std::vector alias works too.
    std::vector<value> vector_items = v.as<std::vector<value>>();

    assert(vector_items.size() == 3);
}

// ==================== Mixed-Type Arrays ====================

void test_mixed_type_arrays() {
    value v = array{1, "Hello", 'a', false, 3.14};

    array items = v.as<array>();

    assert(items[0].as<std::int32_t>() == 1);
    assert(items[1].as<std::string>() == "Hello");
    assert(items[2].as<char>() == 'a');
    assert(items[3].as<bool>() == false);
    assert(items[4].as<double>() == 3.14);
}

// ==================== Nested Arrays ====================

void test_nested_arrays() {
    value v = array{
        1,
        "Hello",
        array{10, 20, 30},
        false
    };

    array items = v.as<array>();
    array nested = items[2].as<array>();

    assert(nested.size() == 3);
    assert(nested[0].as<std::int32_t>() == 10);
    assert(nested[1].as<std::int32_t>() == 20);
    assert(nested[2].as<std::int32_t>() == 30);

    // Test multiple nesting levels.
    value deeply_nested = array{
        array{
            array{100, 200}
        }
    };

    array level1 = deeply_nested.as<array>();
    array level2 = level1[0].as<array>();
    array level3 = level2[0].as<array>();

    assert(level3[0].as<std::int32_t>() == 100);
    assert(level3[1].as<std::int32_t>() == 200);
}

// ==================== Array Copy Behavior ====================

void test_array_copy() {
    value original = array{1, 2, 3};
    value copy = original;

    array original_items = original.as<array>();
    array copy_items = copy.as<array>();

    copy_items[0] = 999;

    assert(original_items[0].as<std::int32_t>() == 1);
    assert(copy_items[0].as<std::int32_t>() == 999);

    // Modifying a retrieved array must not modify stored data.
    assert(copy.as<array>()[0].as<std::int32_t>() == 1);

    // Copy assignment.
    value assigned;
    assigned = original;

    assert(assigned.as<array>()[0].as<std::int32_t>() == 1);
}

// ==================== Array Conversion Rejection ====================

void test_array_conversion_rejection() {
    value arr = array{1, 2, 3};

    // Array to scalar conversions must throw.
    bool threw = false;

    try {
        arr.as<bool>();
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    threw = false;
    try {
        arr.as<std::int32_t>();
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    threw = false;
    try {
        arr.as<double>();
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    threw = false;
    try {
        arr.as<std::string>();
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Scalar to array conversion must throw.
    value scalar = 42;

    threw = false;
    try {
        scalar.as<array>();
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Empty arrays must not participate in truthiness.
    value empty = array{};

    threw = false;
    try {
        empty.as<bool>();
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);
}

// ==================== Array Printing ====================

void test_array_printing() {
    value empty = array{};
    value numbers = array{1, 2, 3};
    value mixed = array{1, "Hello", true};
    value nested = array{
        1,
        array{10, 20},
        "Hello"
    };

    std::ostringstream output;

    output << empty;
    assert(output.str() == "[]");

    output.str("");
    output.clear();

    output << numbers;
    assert(output.str() == "[\n  1,\n  2,\n  3\n]");

    output.str("");
    output.clear();

    output << mixed;
    assert(output.str() == "[\n  1,\n  Hello,\n  true\n]");

    output.str("");
    output.clear();

    output << nested;
    assert(output.str() == "[\n  1,\n  [\n    10,\n    20\n  ],\n  Hello\n]");
}

// ==================== Main Test Function ====================

void test_value_arrays() {
    test_array_constructors();
    test_array_retrieval();
    test_mixed_type_arrays();
    test_nested_arrays();
    test_array_copy();
    test_array_conversion_rejection();
    test_array_printing();
}
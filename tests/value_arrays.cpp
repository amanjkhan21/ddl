#include <cassert>
#include <cstdint>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <ddl/value.hpp>

namespace {

using ddl::array;
using ddl::value;

template <typename Exception, typename Callable>
void assert_throws(Callable&& callable) {
    bool exception_thrown = false;

    try {
        std::forward<Callable>(callable)();
    } catch (const Exception&) {
        exception_thrown = true;
    }

    assert(exception_thrown);
}

void assert_prints(const value& input, const std::string& expected) {
    std::ostringstream output;
    output << input;

    assert(output.str() == expected);
}

// -----------------------------------------------------------------------------
// Array constructors
// -----------------------------------------------------------------------------

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

    // Arrays containing different value types.
    assert(mixed.as<array>()[0].is<std::int32_t>());
    assert(mixed.as<array>()[1].is<std::string>());
    assert(mixed.as<array>()[2].is<char>());
    assert(mixed.as<array>()[3].is<bool>());
    assert(mixed.as<array>()[4].is<double>());
}

// -----------------------------------------------------------------------------
// Array retrieval
// -----------------------------------------------------------------------------

void test_array_retrieval() {
    value v = array{1, 2, 3};

    array items = v.as<array>();

    assert(items.size() == 3);
    assert(items[0].as<std::int32_t>() == 1);
    assert(items[1].as<std::int32_t>() == 2);
    assert(items[2].as<std::int32_t>() == 3);

    // The std::vector alias works too.
    std::vector<value> vector_items = v.as<std::vector<value>>();

    assert(vector_items.size() == 3);
    assert(vector_items[0].as<std::int32_t>() == 1);

    // Retrieving an empty array.
    value empty = array{};
    array empty_items = empty.as<array>();

    assert(empty_items.empty());
}

// -----------------------------------------------------------------------------
// Mixed-type arrays
// -----------------------------------------------------------------------------

void test_mixed_type_arrays() {
    value v = array{1, "Hello", 'a', false, 3.14};

    array items = v.as<array>();

    assert(items[0].as<std::int32_t>() == 1);
    assert(items[1].as<std::string>() == "Hello");
    assert(items[2].as<char>() == 'a');
    assert(items[3].as<bool>() == false);
    assert(items[4].as<double>() == 3.14);
}

// -----------------------------------------------------------------------------
// Nested arrays
// -----------------------------------------------------------------------------

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

    // Multiple nesting levels.
    value deeply_nested = array{
        array{
            array{100, 200}
        }
    };

    array level1 = deeply_nested.as<array>();
    array level2 = level1[0].as<array>();
    array level3 = level2[0].as<array>();

    assert(level3.size() == 2);
    assert(level3[0].as<std::int32_t>() == 100);
    assert(level3[1].as<std::int32_t>() == 200);

    // Empty nested arrays.
    value empty_nested = array{
        array{},
        array{1, 2}
    };

    array empty_nested_items = empty_nested.as<array>();

    assert(empty_nested_items[0].as<array>().empty());
    assert(empty_nested_items[1].as<array>().size() == 2);
}

// -----------------------------------------------------------------------------
// Array copy behavior
// -----------------------------------------------------------------------------

void test_array_copy() {
    value original = array{1, 2, 3};
    value copy = original;

    array original_items = original.as<array>();
    array copy_items = copy.as<array>();

    copy_items[0] = 999;

    // Modifying a retrieved copy does not modify stored data.
    assert(original_items[0].as<std::int32_t>() == 1);
    assert(copy_items[0].as<std::int32_t>() == 999);
    assert(copy.as<array>()[0].as<std::int32_t>() == 1);

    // Copy assignment.
    value assigned;
    assigned = original;

    assert(assigned.is<array>());
    assert(assigned.as<array>()[0].as<std::int32_t>() == 1);

    // Modifying the assigned value does not affect the original.
    assigned.ref<array>()[0] = value(100);

    assert(assigned.ref<array>()[0].as<std::int32_t>() == 100);
    assert(original.ref<array>()[0].as<std::int32_t>() == 1);
}

// -----------------------------------------------------------------------------
// Array reference behavior
// -----------------------------------------------------------------------------

void test_array_references() {
    value v = array{1, 2, 3};

    // Mutable reference.
    array& items = v.ref<array>();

    assert(items.size() == 3);

    items[0] = value(100);

    assert(v.ref<array>()[0].as<std::int32_t>() == 100);

    // Const reference.
    const value& const_v = v;
    const array& const_items = const_v.ref<array>();

    assert(const_items.size() == 3);
    assert(const_items[0].as<std::int32_t>() == 100);

    // Nested array references.
    value nested = array{
        array{10, 20},
        array{30, 40}
    };

    array& outer = nested.ref<array>();
    array& inner = outer[0].ref<array>();

    inner[0] = value(999);

    assert(
        nested.ref<array>()[0].ref<array>()[0]
            .as<std::int32_t>() == 999
    );
}

// -----------------------------------------------------------------------------
// Array conversions
// -----------------------------------------------------------------------------

void test_array_conversion() {
    value arr = array{1, 2, 3};

    // Array to bool conversion.
    assert(arr.as<bool>() == true);

    // Array to other scalar conversions must throw.
    assert_throws<std::invalid_argument>([&]() {
        arr.as<std::int32_t>();
    });

    assert_throws<std::invalid_argument>([&]() {
        arr.as<std::int64_t>();
    });

    assert_throws<std::invalid_argument>([&]() {
        arr.as<float>();
    });

    assert_throws<std::invalid_argument>([&]() {
        arr.as<double>();
    });

    assert_throws<std::invalid_argument>([&]() {
        arr.as<char>();
    });

    assert_throws<std::invalid_argument>([&]() {
        arr.as<std::string>();
    });

    // Scalar to array conversion must throw.
    value scalar = 42;

    assert_throws<std::invalid_argument>([&]() {
        scalar.as<array>();
    });

    // Object to array conversion must throw.
    value object_value = ddl::object{};

    assert_throws<std::invalid_argument>([&]() {
        object_value.as<array>();
    });

    // Empty arrays are truthy.
    value empty = array{};

    assert(empty.as<bool>() == true);
}

// -----------------------------------------------------------------------------
// Array printing
// -----------------------------------------------------------------------------

void test_array_printing() {
    value empty = array{};
    value numbers = array{1, 2, 3};
    value mixed = array{1, "Hello", true};

    value nested = array{
        1,
        array{10, 20},
        "Hello"
    };

    assert_prints(empty, "[]");

    assert_prints(
        numbers,
        "[\n"
        "  1,\n"
        "  2,\n"
        "  3\n"
        "]"
    );

    assert_prints(
        mixed,
        "[\n"
        "  1,\n"
        "  \"Hello\",\n"
        "  true\n"
        "]"
    );

    assert_prints(
        nested,
        "[\n"
        "  1,\n"
        "  [\n"
        "    10,\n"
        "    20\n"
        "  ],\n"
        "  \"Hello\"\n"
        "]"
    );
}

} // namespace

// -----------------------------------------------------------------------------
// Main array test runner
// -----------------------------------------------------------------------------

void test_value_arrays() {
    test_array_constructors();
    test_array_retrieval();
    test_mixed_type_arrays();
    test_nested_arrays();
    test_array_copy();
    test_array_references();
    test_array_conversion();
    test_array_printing();
}
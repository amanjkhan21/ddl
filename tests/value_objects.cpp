#include <cassert>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include <ddl/value.hpp>

using ddl::value;
using ddl::array;
using ddl::object;

// ==================== Object Constructors ====================

void test_object_constructors() {
    value empty = object{};
    value data = object{
        {"name", "Alice"},
        {"age", 25}
    };

    assert(empty.is<object>());
    assert(data.is<object>());

    assert(empty.as<object>().empty());
    assert(data.as<object>().size() == 2);
}

// ==================== Object Retrieval ====================

void test_object_retrieval() {
    value v = object{
        {"name", "Alice"},
        {"age", 25}
    };

    object items = v.as<object>();

    assert(items.size() == 2);
    assert(items.at("name").as<std::string>() == "Alice");
    assert(items.at("age").as<std::int32_t>() == 25);
}

// ==================== Mixed-Type Objects ====================

void test_mixed_type_objects() {
    value v = object{
        {"name", "Alice"},
        {"age", 25},
        {"active", true},
        {"grade", 'A'},
        {"score", 95.5}
    };

    object items = v.as<object>();

    assert(items.at("name").as<std::string>() == "Alice");
    assert(items.at("age").as<std::int32_t>() == 25);
    assert(items.at("active").as<bool>() == true);
    assert(items.at("grade").as<char>() == 'A');
    assert(items.at("score").as<double>() == 95.5);
}

// ==================== Nested Objects ====================

void test_nested_objects() {
    value v = object{
        {"name", "Alice"},
        {"address", object{
            {"city", "Pune"},
            {"country", "India"}
        }}
    };

    object items = v.as<object>();
    object address = items.at("address").as<object>();

    assert(address.at("city").as<std::string>() == "Pune");
    assert(address.at("country").as<std::string>() == "India");
}

// ==================== Arrays Inside Objects ====================

void test_arrays_inside_objects() {
    value v = object{
        {"name", "Alice"},
        {"scores", array{85, 92, 78}}
    };

    object items = v.as<object>();
    array scores = items.at("scores").as<array>();

    assert(scores.size() == 3);
    assert(scores[0].as<std::int32_t>() == 85);
    assert(scores[1].as<std::int32_t>() == 92);
    assert(scores[2].as<std::int32_t>() == 78);
}

// ==================== Objects Inside Arrays ====================

void test_objects_inside_arrays() {
    value v = array{
        object{{"name", "Alice"}},
        object{{"name", "Bob"}}
    };

    array items = v.as<array>();

    assert(items.size() == 2);
    assert(items[0].as<object>().at("name").as<std::string>() == "Alice");
    assert(items[1].as<object>().at("name").as<std::string>() == "Bob");
}

// ==================== Object Copy Behavior ====================

void test_object_copy() {
    value original = object{
        {"name", "Alice"},
        {"age", 25}
    };

    value copy = original;

    object original_items = original.as<object>();
    object copy_items = copy.as<object>();

    copy_items["age"] = 99;

    assert(original_items.at("age").as<std::int32_t>() == 25);
    assert(copy_items.at("age").as<std::int32_t>() == 99);

    // Modifying a retrieved object must not modify stored data.
    assert(copy.as<object>().at("age").as<std::int32_t>() == 25);

    // Copy assignment.
    value assigned;
    assigned = original;

    assert(assigned.as<object>().at("name").as<std::string>() == "Alice");
}

// ==================== Object Conversion Rejection ====================

void test_object_conversion_rejection() {
    value obj = object{{"name", "Alice"}};

    bool threw = false;

    try {
        obj.as<bool>();
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    threw = false;

    try {
        obj.as<std::int32_t>();
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    threw = false;

    try {
        obj.as<double>();
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    threw = false;

    try {
        obj.as<std::string>();
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Scalar to object conversion.
    value scalar = 42;

    threw = false;

    try {
        scalar.as<object>();
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // Array to object conversion.
    value arr = array{1, 2, 3};

    threw = false;

    try {
        arr.as<object>();
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);
}

// ==================== Object Printing ====================

void test_object_printing() {
    value empty = object{};

    std::ostringstream output;
    output << empty;

    assert(output.str() == "{}");

    // Single property: deterministic output.
    output.str("");
    output.clear();

    value single = object{{"name", "Alice"}};
    output << single;

    assert(output.str() == "{\n  \"name\": Alice\n}");

    // Nested object.
    output.str("");
    output.clear();

    value nested = object{
        {"address", object{{"city", "Pune"}}}
    };

    output << nested;

    assert(output.str() ==
        "{\n"
        "  \"address\": {\n"
        "    \"city\": Pune\n"
        "  }\n"
        "}");

    // Array inside object.
    output.str("");
    output.clear();

    value with_array = object{
        {"scores", array{85, 92}}
    };

    output << with_array;

    assert(output.str() ==
        "{\n"
        "  \"scores\": [\n"
        "    85,\n"
        "    92\n"
        "  ]\n"
        "}");
}

// ==================== Main Test Function ====================

void test_value_objects() {
    test_object_constructors();
    test_object_retrieval();
    test_mixed_type_objects();
    test_nested_objects();
    test_arrays_inside_objects();
    test_objects_inside_arrays();
    test_object_copy();
    test_object_conversion_rejection();
    test_object_printing();
}
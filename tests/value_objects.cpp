#include <cassert>
#include <cstdint>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

#include <ddl/value.hpp>

namespace {

using ddl::array;
using ddl::object;
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
// Object constructors
// -----------------------------------------------------------------------------

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

    // Verify stored property types.
    assert(data.ref<object>().at("name").is<std::string>());
    assert(data.ref<object>().at("age").is<std::int32_t>());

    // Object with different value types.
    value mixed = object{
        {"name", "Alice"},
        {"active", true},
        {"score", 95.5}
    };

    assert(mixed.is<object>());
    assert(mixed.as<object>().size() == 3);
}

// -----------------------------------------------------------------------------
// Object retrieval
// -----------------------------------------------------------------------------

void test_object_retrieval() {
    value v = object{
        {"name", "Alice"},
        {"age", 25}
    };

    object items = v.as<object>();

    assert(items.size() == 2);
    assert(items.at("name").as<std::string>() == "Alice");
    assert(items.at("age").as<std::int32_t>() == 25);

    // Retrieve individual values.
    assert(v.ref<object>().at("name").as<std::string>() == "Alice");
    assert(v.ref<object>().at("age").as<std::int32_t>() == 25);

    // Missing keys throw std::out_of_range through at().
    assert_throws<std::out_of_range>([&]() {
        items.at("missing");
    });
}

// -----------------------------------------------------------------------------
// Mixed-type objects
// -----------------------------------------------------------------------------

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

// -----------------------------------------------------------------------------
// Nested objects
// -----------------------------------------------------------------------------

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

    // Multiple nesting levels.
    value deeply_nested = object{
        {"user", object{
            {"profile", object{
                {"name", "Alice"}
            }}
        }}
    };

    object level1 = deeply_nested.as<object>();
    object level2 = level1.at("user").as<object>();
    object level3 = level2.at("profile").as<object>();

    assert(level3.at("name").as<std::string>() == "Alice");
}

// -----------------------------------------------------------------------------
// Arrays inside objects
// -----------------------------------------------------------------------------

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

    // Empty array inside an object.
    value empty_scores = object{
        {"scores", array{}}
    };

    assert(
        empty_scores.ref<object>()
            .at("scores")
            .as<array>()
            .empty()
    );
}

// -----------------------------------------------------------------------------
// Objects inside arrays
// -----------------------------------------------------------------------------

void test_objects_inside_arrays() {
    value v = array{
        object{{"name", "Alice"}},
        object{{"name", "Bob"}}
    };

    array items = v.as<array>();

    assert(items.size() == 2);
    assert(items[0].as<object>().at("name").as<std::string>() == "Alice");
    assert(items[1].as<object>().at("name").as<std::string>() == "Bob");

    // Access nested objects through references.
    assert(
        v.ref<array>()[0].ref<object>()
            .at("name")
            .as<std::string>() == "Alice"
    );
}

// -----------------------------------------------------------------------------
// Object copy behavior
// -----------------------------------------------------------------------------

void test_object_copy() {
    value original = object{
        {"name", "Alice"},
        {"age", 25}
    };

    value copy = original;

    object original_items = original.as<object>();
    object copy_items = copy.as<object>();

    copy_items["age"] = 99;

    // Retrieved objects are independent copies.
    assert(original_items.at("age").as<std::int32_t>() == 25);
    assert(copy_items.at("age").as<std::int32_t>() == 99);

    // Modifying a retrieved object does not modify stored data.
    assert(copy.as<object>().at("age").as<std::int32_t>() == 25);

    // Copy assignment.
    value assigned;
    assigned = original;

    assert(assigned.is<object>());
    assert(assigned.as<object>().at("name").as<std::string>() == "Alice");

    // Modifying the assigned object must not affect the original.
    assigned.ref<object>().at("name") = value("Bob");

    assert(
        assigned.ref<object>().at("name").as<std::string>() == "Bob"
    );

    assert(
        original.ref<object>().at("name").as<std::string>() == "Alice"
    );
}

// -----------------------------------------------------------------------------
// Object reference behavior
// -----------------------------------------------------------------------------

void test_object_references() {
    value v = object{
        {"name", "Alice"},
        {"age", 25}
    };

    // Mutable reference.
    object& items = v.ref<object>();

    assert(items.size() == 2);

    items.at("name") = value("Bob");

    assert(
        v.ref<object>().at("name").as<std::string>() == "Bob"
    );

    // Add a new property.
    items["active"] = value(true);

    assert(v.ref<object>().at("active").as<bool>());

    // Const reference.
    const value& const_v = v;
    const object& const_items = const_v.ref<object>();

    assert(const_items.size() == 3);
    assert(
        const_items.at("name").as<std::string>() == "Bob"
    );

    // Nested object references.
    value nested = object{
        {"user", object{
            {"name", "Alice"}
        }}
    };

    object& outer = nested.ref<object>();
    object& inner = outer.at("user").ref<object>();

    inner.at("name") = value("Charlie");

    assert(
        nested.ref<object>()
            .at("user")
            .ref<object>()
            .at("name")
            .as<std::string>() == "Charlie"
    );
}

// -----------------------------------------------------------------------------
// Object conversions
// -----------------------------------------------------------------------------

void test_object_conversion() {
    value obj = object{
        {"name", "Alice"}
    };

    // Object to bool conversion.
    assert(obj.as<bool>() == true);

    // Object to other scalar conversions must throw.
    assert_throws<std::invalid_argument>([&]() {
        obj.as<std::int32_t>();
    });

    assert_throws<std::invalid_argument>([&]() {
        obj.as<std::int64_t>();
    });

    assert_throws<std::invalid_argument>([&]() {
        obj.as<float>();
    });

    assert_throws<std::invalid_argument>([&]() {
        obj.as<double>();
    });

    assert_throws<std::invalid_argument>([&]() {
        obj.as<char>();
    });

    assert_throws<std::invalid_argument>([&]() {
        obj.as<std::string>();
    });

    // Scalar to object conversion must throw.
    value scalar = 42;

    assert_throws<std::invalid_argument>([&]() {
        scalar.as<object>();
    });

    // Array to object conversion must throw.
    value arr = array{1, 2, 3};

    assert_throws<std::invalid_argument>([&]() {
        arr.as<object>();
    });

    // Empty objects are truthy.
    value empty = object{};

    assert(empty.as<bool>() == true);
}

// -----------------------------------------------------------------------------
// Object printing
// -----------------------------------------------------------------------------

void test_object_printing() {
    // Empty object.
    assert_prints(value(object{}), "{}");

    // Single property: deterministic output.
    assert_prints(
        value(object{
            {"name", "Alice"}
        }),
        "{\n"
        "  \"name\": \"Alice\"\n"
        "}"
    );

    // Nested object.
    assert_prints(
        value(object{
            {"address", object{
                {"city", "Pune"}
            }}
        }),
        "{\n"
        "  \"address\": {\n"
        "    \"city\": \"Pune\"\n"
        "  }\n"
        "}"
    );

    // Array inside object.
    assert_prints(
        value(object{
            {"scores", array{85, 92}}
        }),
        "{\n"
        "  \"scores\": [\n"
        "    85,\n"
        "    92\n"
        "  ]\n"
        "}"
    );
}

} // namespace

// -----------------------------------------------------------------------------
// Main object test runner
// -----------------------------------------------------------------------------

void test_value_objects() {
    test_object_constructors();
    test_object_retrieval();
    test_mixed_type_objects();
    test_nested_objects();
    test_arrays_inside_objects();
    test_objects_inside_arrays();
    test_object_copy();
    test_object_references();
    test_object_conversion();
    test_object_printing();
}
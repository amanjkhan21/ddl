#include <cassert>
#include <cstdint>
#include <string>
#include <utility>

#include <ddl/value.hpp>

// -----------------------------------------------------------------------------
// Copy construction
// -----------------------------------------------------------------------------

void test_value_copy_construction() {
    using namespace ddl;

    value original(std::string("hello"));
    value copy(original);

    assert(copy.is<std::string>());
    assert(copy.as<std::string>() == "hello");

    // Changing the copy must not affect the original.
    copy = value("world");

    assert(original.as<std::string>() == "hello");
    assert(copy.as<std::string>() == "world");

    // Copy an array.
    value original_array(array{
        value(1),
        value(2),
        value(3)
    });

    value copied_array(original_array);

    assert(copied_array.is<array>());
    assert(copied_array.as<array>().size() == 3);

    copied_array.ref<array>()[0] = value(99);

    assert(original_array.ref<array>()[0].as<std::int32_t>() == 1);
    assert(copied_array.ref<array>()[0].as<std::int32_t>() == 99);

    // Copy an object.
    value original_object(object{
        {"name", value("DDL")}
    });

    value copied_object(original_object);

    assert(copied_object.is<object>());
    assert(copied_object.ref<object>().at("name").as<std::string>() == "DDL");

    copied_object.ref<object>().at("name") = value("Modified");

    assert(original_object.ref<object>().at("name").as<std::string>() == "DDL");
    assert(copied_object.ref<object>().at("name").as<std::string>() == "Modified");
}

// -----------------------------------------------------------------------------
// Copy assignment
// -----------------------------------------------------------------------------

void test_value_copy_assignment() {
    using namespace ddl;

    value original(42);
    value copy("initial");

    copy = original;

    assert(copy.is<std::int32_t>());
    assert(copy.as<std::int32_t>() == 42);

    copy = value(100);

    assert(original.as<std::int32_t>() == 42);
    assert(copy.as<std::int32_t>() == 100);

    // Self-copy assignment.
    original = original;

    assert(original.is<std::int32_t>());
    assert(original.as<std::int32_t>() == 42);

    // Copy assignment of an array.
    value original_array(array{
        value("first"),
        value("second")
    });

    value copied_array;
    copied_array = original_array;

    assert(copied_array.is<array>());
    assert(copied_array.ref<array>().size() == 2);

    copied_array.ref<array>()[0] = value("changed");

    assert(original_array.ref<array>()[0].as<std::string>() == "first");

    // Copy assignment of an object.
    value original_object(object{
        {"enabled", value(true)}
    });

    value copied_object;
    copied_object = original_object;

    assert(copied_object.is<object>());
    assert(copied_object.ref<object>().at("enabled").as<bool>());

    copied_object.ref<object>().at("enabled") = value(false);

    assert(original_object.ref<object>().at("enabled").as<bool>());
}

// -----------------------------------------------------------------------------
// Move construction
// -----------------------------------------------------------------------------

void test_value_move_construction() {
    using namespace ddl;

    value original(std::string("hello"));
    value moved(std::move(original));

    assert(moved.is<std::string>());
    assert(moved.as<std::string>() == "hello");

    // Move an array.
    value original_array(array{
        value(1),
        value(2),
        value(3)
    });

    value moved_array(std::move(original_array));

    assert(moved_array.is<array>());
    assert(moved_array.ref<array>().size() == 3);
    assert(moved_array.ref<array>()[0].as<std::int32_t>() == 1);

    // Move an object.
    value original_object(object{
        {"name", value("DDL")}
    });

    value moved_object(std::move(original_object));

    assert(moved_object.is<object>());
    assert(moved_object.ref<object>().at("name").as<std::string>() == "DDL");
}

// -----------------------------------------------------------------------------
// Move assignment
// -----------------------------------------------------------------------------

void test_value_move_assignment() {
    using namespace ddl;

    value original(std::string("hello"));
    value destination("initial");

    destination = std::move(original);

    assert(destination.is<std::string>());
    assert(destination.as<std::string>() == "hello");

    // Move assignment of an array.
    value original_array(array{
        value("first"),
        value("second")
    });

    value destination_array;
    destination_array = std::move(original_array);

    assert(destination_array.is<array>());
    assert(destination_array.ref<array>().size() == 2);
    assert(destination_array.ref<array>()[1].as<std::string>() == "second");

    // Move assignment of an object.
    value original_object(object{
        {"version", value(std::int32_t{1})}
    });

    value destination_object;
    destination_object = std::move(original_object);

    assert(destination_object.is<object>());
    assert(
        destination_object.ref<object>()
            .at("version")
            .as<std::int32_t>() == 1
    );
}

// -----------------------------------------------------------------------------
// Main copy and move test runner
// -----------------------------------------------------------------------------

void test_value_copy_move() {
    test_value_copy_construction();
    test_value_copy_assignment();
    test_value_move_construction();
    test_value_move_assignment();
}
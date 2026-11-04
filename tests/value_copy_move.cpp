#include <cassert>
#include <string>
#include <utility>
#include <ddl/value.hpp>

void test_value_copy_construction() {
    ddl::value original(std::string("hello"));
    ddl::value copy(original);

    assert(copy.is<std::string>());
    assert(copy.as<std::string>() == "hello");

    // Changing the copy should not change the original.
    copy = ddl::value("world");

    assert(original.as<std::string>() == "hello");
    assert(copy.as<std::string>() == "world");
}

void test_value_copy_assignment() {
    ddl::value original(42);
    ddl::value copy("initial");

    copy = original;

    assert(copy.is<std::int32_t>());
    assert(copy.as<std::int32_t>() == 42);

    copy = ddl::value(100);

    assert(original.as<std::int32_t>() == 42);
    assert(copy.as<std::int32_t>() == 100);
}

void test_value_move_construction() {
    ddl::value original(std::string("hello"));
    ddl::value moved(std::move(original));

    assert(moved.is<std::string>());
    assert(moved.as<std::string>() == "hello");
}

void test_value_move_assignment() {
    ddl::value original(std::string("hello"));
    ddl::value destination("initial");

    destination = std::move(original);

    assert(destination.is<std::string>());
    assert(destination.as<std::string>() == "hello");
}

void test_value_copy_move() {
    test_value_copy_construction();
    test_value_copy_assignment();
    test_value_move_construction();
    test_value_move_assignment();
}
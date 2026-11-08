#include <ddl/value.hpp>

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>

namespace {

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

template <typename T>
void assert_reference_mismatch(const ddl::value& v) {
    assert_throws<std::invalid_argument>([&]() {
        v.ref<T>();
    });
}

// -------------------------------------------------------------------------
// Non-const references
// -------------------------------------------------------------------------

void test_non_const_value_references() {
    ddl::value v = std::int32_t(42);

    std::int32_t& number = v.ref<std::int32_t>();

    assert(number == 42);

    number = 100;

    assert(v.as<std::int32_t>() == 100);

    // Repeated calls must refer to the same stored value.
    assert(&number == &v.ref<std::int32_t>());
}

// -------------------------------------------------------------------------
// Const references
// -------------------------------------------------------------------------

void test_const_value_references() {
    const ddl::value v = std::int32_t(42);

    const std::int32_t& number = v.ref<std::int32_t>();

    assert(number == 42);
    assert(&number == &v.ref<std::int32_t>());
}

// -------------------------------------------------------------------------
// Exact references to every supported variant alternative
// -------------------------------------------------------------------------

void test_all_supported_type_references() {
    ddl::value null_value(nullptr);
    assert(null_value.ref<std::nullptr_t>() == nullptr);

    ddl::value bool_value(true);
    assert(bool_value.ref<bool>());

    ddl::value int32_value(std::int32_t(42));
    assert(int32_value.ref<std::int32_t>() == 42);

    ddl::value int64_value(std::int64_t(42000));
    assert(int64_value.ref<std::int64_t>() == 42000);

    ddl::value float_value(3.14f);
    assert(float_value.ref<float>() == 3.14f);

    ddl::value double_value(6.28);
    assert(double_value.ref<double>() == 6.28);

    ddl::value char_value('A');
    assert(char_value.ref<char>() == 'A');

    ddl::value string_value(std::string("hello"));
    assert(string_value.ref<std::string>() == "hello");

    ddl::value array_value(ddl::array{1, 2, 3});
    assert(array_value.ref<ddl::array>().size() == 3);

    ddl::value object_value(
        ddl::object{
            {"answer", ddl::value(std::int32_t(42))}
        }
    );

    assert(object_value.ref<ddl::object>().size() == 1);
    assert(
        object_value.ref<ddl::object>()
            .at("answer")
            .as<std::int32_t>() == 42
    );
}

// -------------------------------------------------------------------------
// Reference mutation
// -------------------------------------------------------------------------

void test_reference_mutation() {
    ddl::value bool_value(false);
    bool_value.ref<bool>() = true;
    assert(bool_value.as<bool>());

    ddl::value int64_value(std::int64_t(100));
    int64_value.ref<std::int64_t>() = 200;
    assert(int64_value.as<std::int64_t>() == 200);

    ddl::value float_value(1.5f);
    float_value.ref<float>() = 2.5f;
    assert(float_value.as<float>() == 2.5f);

    ddl::value double_value(1.5);
    double_value.ref<double>() = 2.5;
    assert(double_value.as<double>() == 2.5);

    ddl::value char_value('A');
    char_value.ref<char>() = 'B';
    assert(char_value.as<char>() == 'B');

    ddl::value string_value(std::string("hello"));
    string_value.ref<std::string>() = "world";
    assert(string_value.as<std::string>() == "world");
}

// -------------------------------------------------------------------------
// Value self-reference
// -------------------------------------------------------------------------

void test_value_self_reference() {
    ddl::value v = std::int32_t(42);

    ddl::value& self = v.ref<ddl::value>();

    self = std::string("hello");

    assert(v.is<std::string>());
    assert(v.as<std::string>() == "hello");
    assert(&self == &v);
}

// -------------------------------------------------------------------------
// Const value self-reference
// -------------------------------------------------------------------------

void test_const_value_self_reference() {
    const ddl::value v = std::int32_t(42);

    const ddl::value& self = v.ref<ddl::value>();

    assert(&self == &v);
    assert(self.as<std::int32_t>() == 42);
}

// -------------------------------------------------------------------------
// Runtime type mismatch
// -------------------------------------------------------------------------

void test_reference_type_mismatch() {
    ddl::value v = std::int32_t(42);

    assert_reference_mismatch<std::nullptr_t>(v);
    assert_reference_mismatch<bool>(v);
    assert_reference_mismatch<std::int64_t>(v);
    assert_reference_mismatch<float>(v);
    assert_reference_mismatch<double>(v);
    assert_reference_mismatch<char>(v);
    assert_reference_mismatch<std::string>(v);
    assert_reference_mismatch<ddl::array>(v);
    assert_reference_mismatch<ddl::object>(v);
}

// -------------------------------------------------------------------------
// Array references
// -------------------------------------------------------------------------

void test_array_references() {
    ddl::value v = ddl::array{
        ddl::value(10),
        ddl::value(20)
    };

    ddl::array& arr = v.ref<ddl::array>();

    assert(arr.size() == 2);

    arr[0].ref<std::int32_t>() = 99;

    assert(v.ref<ddl::array>()[0].as<std::int32_t>() == 99);

    arr.push_back(30);

    assert(v.ref<ddl::array>().size() == 3);
    assert(v.ref<ddl::array>()[2].as<std::int32_t>() == 30);
}

// -------------------------------------------------------------------------
// Object references
// -------------------------------------------------------------------------

void test_object_references() {
    ddl::value v = ddl::object{
        {"name", ddl::value("Alice")},
        {"age", ddl::value(std::int32_t(20))}
    };

    ddl::object& obj = v.ref<ddl::object>();

    assert(obj.size() == 2);

    obj.at("age").ref<std::int32_t>() = 21;

    assert(v.ref<ddl::object>().at("age").as<std::int32_t>() == 21);

    obj.emplace("active", true);

    assert(v.ref<ddl::object>().at("active").as<bool>());
}

// -------------------------------------------------------------------------
// Nested references
// -------------------------------------------------------------------------

void test_nested_references() {
    ddl::value v = ddl::array{
        ddl::value(10),
        ddl::value(20)
    };

    ddl::array& arr = v.ref<ddl::array>();

    std::int32_t& number = arr[0].ref<std::int32_t>();

    number = 99;

    assert(v.ref<ddl::array>()[0].as<std::int32_t>() == 99);
}

// -------------------------------------------------------------------------
// Deep nested references
// -------------------------------------------------------------------------

void test_deeply_nested_references() {
    ddl::value v = ddl::object{
        {
            "users",
            ddl::value(ddl::array{
                ddl::value(ddl::object{
                    {"age", ddl::value(std::int32_t(18))}
                })
            })
        }
    };

    ddl::object& root = v.ref<ddl::object>();
    ddl::array& users = root.at("users").ref<ddl::array>();
    ddl::object& user = users[0].ref<ddl::object>();

    user.at("age").ref<std::int32_t>() = 25;

    assert(
        v.ref<ddl::object>()
            .at("users")
            .ref<ddl::array>()[0]
            .ref<ddl::object>()
            .at("age")
            .as<std::int32_t>() == 25
    );
}

// -------------------------------------------------------------------------
// Const container references
// -------------------------------------------------------------------------

void test_const_container_references() {
    const ddl::value v = ddl::array{
        ddl::value(10),
        ddl::value(20)
    };

    const ddl::array& arr = v.ref<ddl::array>();

    assert(arr.size() == 2);
    assert(arr[0].as<std::int32_t>() == 10);

    static_assert(
        std::is_const_v<
            std::remove_reference_t<
                decltype(v.ref<ddl::array>())
            >
        >
    );
}

// -------------------------------------------------------------------------
// Compile-time return type checks
// -------------------------------------------------------------------------

void test_reference_return_types() {
    static_assert(std::is_same_v<
        decltype(std::declval<ddl::value&>().ref<std::int32_t>()),
        std::int32_t&
    >);

    static_assert(std::is_same_v<
        decltype(std::declval<const ddl::value&>().ref<std::int32_t>()),
        const std::int32_t&
    >);

    static_assert(std::is_same_v<
        decltype(std::declval<ddl::value&>().ref<ddl::value>()),
        ddl::value&
    >);

    static_assert(std::is_same_v<
        decltype(std::declval<const ddl::value&>().ref<ddl::value>()),
        const ddl::value&
    >);
}

} // namespace

// -------------------------------------------------------------------------
// Main reference test runner
// -------------------------------------------------------------------------

void test_value_references() {
    test_non_const_value_references();
    test_const_value_references();

    test_all_supported_type_references();
    test_reference_mutation();

    test_value_self_reference();
    test_const_value_self_reference();

    test_reference_type_mismatch();

    test_array_references();
    test_object_references();

    test_nested_references();
    test_deeply_nested_references();

    test_const_container_references();
    test_reference_return_types();
}
#include <cassert>
#include <cmath>
#include <cstdint>
#include <limits>
#include <stdexcept>
#include <string>
#include <utility>

#include <ddl/value.hpp>

// -----------------------------------------------------------------------------
// Test helpers
// -----------------------------------------------------------------------------

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

// -----------------------------------------------------------------------------
// Boolean conversions
// -----------------------------------------------------------------------------

void test_bool_conversion() {
    using namespace ddl;

    // nullptr
    assert(value(nullptr).as<bool>() == false);

    // bool
    assert(value(true).as<bool>() == true);
    assert(value(false).as<bool>() == false);

    // Integers
    assert(value(0).as<bool>() == false);
    assert(value(42).as<bool>() == true);
    assert(value(-42).as<bool>() == true);

    // Floating-point
    assert(value(0.0f).as<bool>() == false);
    assert(value(3.14f).as<bool>() == true);
    assert(value(0.0).as<bool>() == false);
    assert(value(3.14).as<bool>() == true);

    // NaN
    assert(value(std::numeric_limits<double>::quiet_NaN()).as<bool>() == true);

    // char
    assert(value('\0').as<bool>() == false);
    assert(value('A').as<bool>() == true);

    // string
    assert(value("").as<bool>() == false);
    assert(value("hello").as<bool>() == true);
    assert(value("false").as<bool>() == true);

    // array
    assert(value(array{}).as<bool>() == true);
    assert(value(array{1, 2, 3}).as<bool>() == true);

    // object
    assert(value(object{}).as<bool>() == true);
    assert(value(object{{"answer", 42}}).as<bool>() == true);

    // Infinity
    assert(value(std::numeric_limits<double>::infinity()).as<bool>());
    assert(value(-std::numeric_limits<double>::infinity()).as<bool>());
}

// -----------------------------------------------------------------------------
// 32-bit integer conversions
// -----------------------------------------------------------------------------

void test_int32_conversion() {
    using namespace ddl;

    // nullptr and bool
    assert(value(nullptr).as<std::int32_t>() == 0);
    assert(value(true).as<std::int32_t>() == 1);
    assert(value(false).as<std::int32_t>() == 0);

    // Integers
    assert(value(std::int32_t{42}).as<std::int32_t>() == 42);
    assert(value(std::int64_t{42}).as<std::int32_t>() == 42);

    // Floating-point truncation
    assert(value(3.9f).as<std::int32_t>() == 3);
    assert(value(-3.9f).as<std::int32_t>() == -3);
    assert(value(3.9).as<std::int32_t>() == 3);
    assert(value(-3.9).as<std::int32_t>() == -3);

    // char
    assert_throws<std::invalid_argument>([]() {
        value('A').as<std::int32_t>();
    });

    // int64_t -> int32_t: valid boundaries
    value int64_max(
        static_cast<std::int64_t>(
            std::numeric_limits<std::int32_t>::max()
        )
    );

    value int64_min(
        static_cast<std::int64_t>(
            std::numeric_limits<std::int32_t>::min()
        )
    );

    assert(
        int64_max.as<std::int32_t>() ==
        std::numeric_limits<std::int32_t>::max()
    );

    assert(
        int64_min.as<std::int32_t>() ==
        std::numeric_limits<std::int32_t>::min()
    );

    // int64_t -> int32_t: overflow above maximum
    value int64_above_max(
        static_cast<std::int64_t>(
            std::numeric_limits<std::int32_t>::max()
        ) + 1
    );

    assert_throws<std::out_of_range>([&]() {
        int64_above_max.as<std::int32_t>();
    });

    // int64_t -> int32_t: overflow below minimum
    value int64_below_min(
        static_cast<std::int64_t>(
            std::numeric_limits<std::int32_t>::min()
        ) - 1
    );

    assert_throws<std::out_of_range>([&]() {
        int64_below_min.as<std::int32_t>();
    });

    // double -> int32_t: valid lower boundary
    value double_min(-2147483648.0);

    assert(
        double_min.as<std::int32_t>() ==
        std::numeric_limits<std::int32_t>::min()
    );

    // double -> int32_t: overflow above maximum
    value double_upper(2147483648.0);

    assert_throws<std::out_of_range>([&]() {
        double_upper.as<std::int32_t>();
    });

    // Fractional values near the boundaries
    value positive_fraction(2147483647.9);
    value negative_fraction(-2147483648.9);

    assert(positive_fraction.as<std::int32_t>() == 2147483647);
    assert(negative_fraction.as<std::int32_t>() == -2147483648);

    // double -> int32_t: below truncation-based lower boundary
    value double_below_min(-2147483649.0);

    assert_throws<std::out_of_range>([&]() {
        double_below_min.as<std::int32_t>();
    });

    // float -> int32_t: valid lower boundary
    assert(
        value(-2147483648.0f).as<std::int32_t>() ==
        std::numeric_limits<std::int32_t>::min()
    );

    // float -> int32_t: overflow above maximum
    assert_throws<std::out_of_range>([]() {
        value(2147483648.0f).as<std::int32_t>();
    });

    // NaN
    assert_throws<std::out_of_range>([]() {
        value(std::numeric_limits<double>::quiet_NaN())
            .as<std::int32_t>();
    });

    // Infinity
    assert_throws<std::out_of_range>([]() {
        value(std::numeric_limits<float>::infinity())
            .as<std::int32_t>();
    });

    // Unsupported conversion
    assert_throws<std::invalid_argument>([]() {
        value("42").as<std::int32_t>();
    });
}

// -----------------------------------------------------------------------------
// 64-bit integer conversions
// -----------------------------------------------------------------------------

void test_int64_conversion() {
    using namespace ddl;

    // nullptr and bool
    assert(value(nullptr).as<std::int64_t>() == 0);
    assert(value(true).as<std::int64_t>() == 1);
    assert(value(false).as<std::int64_t>() == 0);

    // Integers
    assert(value(std::int32_t{42}).as<std::int64_t>() == 42);
    assert(value(std::int64_t{42}).as<std::int64_t>() == 42);

    // Floating-point truncation
    assert(value(3.9f).as<std::int64_t>() == 3);
    assert(value(-3.9f).as<std::int64_t>() == -3);
    assert(value(3.9).as<std::int64_t>() == 3);
    assert(value(-3.9).as<std::int64_t>() == -3);

    // char
    assert_throws<std::invalid_argument>([]() {
        value('A').as<std::int64_t>();
    });

    // double -> int64_t: valid lower boundary
    value double_min(-9223372036854775808.0);

    assert(
        double_min.as<std::int64_t>() ==
        std::numeric_limits<std::int64_t>::min()
    );

    // double -> int64_t: overflow above maximum
    value double_upper(9223372036854775808.0);

    assert_throws<std::out_of_range>([&]() {
        double_upper.as<std::int64_t>();
    });

    // float -> int64_t: valid lower boundary
    value float_min(-9223372036854775808.0f);

    assert(
        float_min.as<std::int64_t>() ==
        std::numeric_limits<std::int64_t>::min()
    );

    // float -> int64_t: overflow above maximum
    value float_upper(9223372036854775808.0f);

    assert_throws<std::out_of_range>([&]() {
        float_upper.as<std::int64_t>();
    });

    // NaN
    assert_throws<std::out_of_range>([]() {
        value(std::numeric_limits<double>::quiet_NaN())
            .as<std::int64_t>();
    });

    // Positive infinity
    assert_throws<std::out_of_range>([]() {
        value(std::numeric_limits<double>::infinity())
            .as<std::int64_t>();
    });

    // Negative infinity
    assert_throws<std::out_of_range>([]() {
        value(-std::numeric_limits<double>::infinity())
            .as<std::int64_t>();
    });

    // Unsupported conversion
    assert_throws<std::invalid_argument>([]() {
        value("42").as<std::int64_t>();
    });
}

// -----------------------------------------------------------------------------
// Float conversions
// -----------------------------------------------------------------------------

void test_float_conversion() {
    using namespace ddl;

    // nullptr and bool
    assert(value(nullptr).as<float>() == 0.0f);
    assert(value(true).as<float>() == 1.0f);
    assert(value(false).as<float>() == 0.0f);

    // Integers
    assert(value(std::int32_t{42}).as<float>() == 42.0f);
    assert(value(std::int64_t{123456}).as<float>() == 123456.0f);

    // int64_t -> float: precision loss
    std::int64_t large_int64 = 16777217LL;

    assert(
        value(large_int64).as<float>() ==
        static_cast<float>(large_int64)
    );

    assert(
        value(large_int64).as<float>() !=
        static_cast<float>(16777217LL + 1)
    );

    // float
    assert(value(3.14f).as<float>() == 3.14f);

    // double -> float: precision loss
    assert(value(3.14).as<float>() == static_cast<float>(3.14));

    // char
    assert_throws<std::invalid_argument>([]() {
        value('A').as<float>();
    });

    // NaN is preserved
    assert(std::isnan(
        value(std::numeric_limits<double>::quiet_NaN()).as<float>()
    ));

    // Positive infinity is preserved
    assert(std::isinf(
        value(std::numeric_limits<double>::infinity()).as<float>()
    ));

    // Negative infinity is preserved
    assert(std::isinf(
        value(-std::numeric_limits<double>::infinity()).as<float>()
    ));

    // Positive overflow
    assert_throws<std::out_of_range>([]() {
        value(std::numeric_limits<double>::max()).as<float>();
    });

    // Negative overflow
    assert_throws<std::out_of_range>([]() {
        value(-std::numeric_limits<double>::max()).as<float>();
    });

    // Underflow
    assert(value(1e-100).as<float>() == 0.0f);

    // Unsupported conversion
    assert_throws<std::invalid_argument>([]() {
        value("3.14").as<float>();
    });
}

// -----------------------------------------------------------------------------
// Double conversions
// -----------------------------------------------------------------------------

void test_double_conversion() {
    using namespace ddl;

    // nullptr and bool
    assert(value(nullptr).as<double>() == 0.0);
    assert(value(true).as<double>() == 1.0);
    assert(value(false).as<double>() == 0.0);

    // Integers
    assert(value(std::int32_t{42}).as<double>() == 42.0);
    assert(value(std::int64_t{123456789}).as<double>() == 123456789.0);

    // int64_t -> double: precision loss
    std::int64_t large_int64 = 9007199254740993LL;

    assert(
        value(large_int64).as<double>() ==
        static_cast<double>(large_int64)
    );

    // float -> double
    assert(value(3.14f).as<double>() == static_cast<double>(3.14f));

    // double
    assert(value(3.14).as<double>() == 3.14);

    // char
    assert_throws<std::invalid_argument>([]() {
        value('A').as<double>();
    });

    // NaN is preserved
    assert(std::isnan(
        value(std::numeric_limits<double>::quiet_NaN()).as<double>()
    ));

    // Positive infinity is preserved
    assert(std::isinf(
        value(std::numeric_limits<double>::infinity()).as<double>()
    ));

    // Negative infinity is preserved
    assert(std::isinf(
        value(-std::numeric_limits<double>::infinity()).as<double>()
    ));

    // Unsupported conversion
    assert_throws<std::invalid_argument>([]() {
        value("3.14").as<double>();
    });
}

// -----------------------------------------------------------------------------
// Character conversions
// -----------------------------------------------------------------------------

void test_char_conversion() {
    using namespace ddl;

    // Exact character conversions
    assert(value('A').as<char>() == 'A');
    assert(value('z').as<char>() == 'z');
    assert(value('\0').as<char>() == '\0');

    // Unsupported conversion
    assert_throws<std::invalid_argument>([]() {
        value(65).as<char>();
    });
}

// -----------------------------------------------------------------------------
// String conversions
// -----------------------------------------------------------------------------

void test_string_conversion() {
    using namespace ddl;

    // nullptr
    assert(value(nullptr).as<std::string>() == "null");

    // bool
    assert(value(true).as<std::string>() == "true");
    assert(value(false).as<std::string>() == "false");

    // Integers
    assert(value(std::int32_t{42}).as<std::string>() == "42");
    assert(value(std::int64_t{123456789}).as<std::string>() == "123456789");

    // char
    assert(value('A').as<std::string>() == "A");
    assert(value('\0').as<std::string>() == std::string(1, '\0'));

    // string
    assert(value(std::string("hello")).as<std::string>() == "hello");
    assert(value("ddlamic").as<std::string>() == "ddlamic");

    // Floating-point
    assert(value(3.14f).as<std::string>() == "3.140000");
    assert(value(3.14).as<std::string>() == "3.140000");
}

} // namespace

// -----------------------------------------------------------------------------
// Main conversion test runner
// -----------------------------------------------------------------------------

void test_value_conversions() {
    test_bool_conversion();
    test_int32_conversion();
    test_int64_conversion();
    test_float_conversion();
    test_double_conversion();
    test_char_conversion();
    test_string_conversion();
}
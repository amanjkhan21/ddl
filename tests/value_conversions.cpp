#include <cassert>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <string>
#include <cstdint>
#include <ddl/value.hpp>

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
    assert(value(std::numeric_limits<double>::quiet_NaN()).as<bool>() == true);

    // char
    assert(value('\0').as<bool>() == false);
    assert(value('A').as<bool>() == true);

    // string
    assert(value("").as<bool>() == false);
    assert(value("hello").as<bool>() == true);
    assert(value("false").as<bool>() == true);
}

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
    assert(value('A').as<std::int32_t>() ==
           static_cast<std::int32_t>('A'));

    // int64_t -> int32_t boundaries
    value int64_max(
        static_cast<std::int64_t>(std::numeric_limits<std::int32_t>::max())
    );

    value int64_min(
        static_cast<std::int64_t>(std::numeric_limits<std::int32_t>::min())
    );

    assert(int64_max.as<std::int32_t>() ==
           std::numeric_limits<std::int32_t>::max());

    assert(int64_min.as<std::int32_t>() ==
           std::numeric_limits<std::int32_t>::min());

    // int64_t overflow: above maximum
    value int64_above_max(
        static_cast<std::int64_t>(std::numeric_limits<std::int32_t>::max()) + 1
    );

    bool exception_thrown = false;

    try {
        int64_above_max.as<std::int32_t>();
    } catch (const std::out_of_range&) {
        exception_thrown = true;
    }

    assert(exception_thrown);

    // int64_t overflow: below minimum
    value int64_below_min(
        static_cast<std::int64_t>(std::numeric_limits<std::int32_t>::min()) - 1
    );

    exception_thrown = false;

    try {
        int64_below_min.as<std::int32_t>();
    } catch (const std::out_of_range&) {
        exception_thrown = true;
    }

    assert(exception_thrown);

    // double -> int32_t boundaries
    value double_min(-2147483648.0);

    assert(double_min.as<std::int32_t>() ==
           std::numeric_limits<std::int32_t>::min());

    value double_upper(2147483648.0);

    exception_thrown = false;

    try {
        double_upper.as<std::int32_t>();
    } catch (const std::out_of_range&) {
        exception_thrown = true;
    }

    assert(exception_thrown);

    // Fractional values near the boundaries
    value positive_fraction(2147483647.9);
    assert(positive_fraction.as<std::int32_t>() == 2147483647);

    value negative_fraction(-2147483648.9);
    assert(negative_fraction.as<std::int32_t>() == -2147483648);

    // Below the truncation-based lower boundary
    value double_below_min(-2147483649.0);

    exception_thrown = false;

    try {
        double_below_min.as<std::int32_t>();
    } catch (const std::out_of_range&) {
        exception_thrown = true;
    }

    assert(exception_thrown);

    // float -> int32_t boundaries
    assert(value(-2147483648.0f).as<std::int32_t>() ==
           std::numeric_limits<std::int32_t>::min());

    exception_thrown = false;

    try {
        value(2147483648.0f).as<std::int32_t>();
    } catch (const std::out_of_range&) {
        exception_thrown = true;
    }

    assert(exception_thrown);

    // NaN and infinity
    exception_thrown = false;

    try {
        value(std::numeric_limits<double>::quiet_NaN())
            .as<std::int32_t>();
    } catch (const std::out_of_range&) {
        exception_thrown = true;
    }

    assert(exception_thrown);

    exception_thrown = false;

    try {
        value(std::numeric_limits<float>::infinity())
            .as<std::int32_t>();
    } catch (const std::out_of_range&) {
        exception_thrown = true;
    }

    assert(exception_thrown);

    // Unsupported conversion
    exception_thrown = false;

    try {
        value("42").as<std::int32_t>();
    } catch (const std::invalid_argument&) {
        exception_thrown = true;
    }

    assert(exception_thrown);
}

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
    assert(value('A').as<std::int64_t>() ==
           static_cast<std::int64_t>('A'));

    // double -> int64_t boundaries
    value double_min(-9223372036854775808.0);

    assert(double_min.as<std::int64_t>() ==
           std::numeric_limits<std::int64_t>::min());

    value double_upper(9223372036854775808.0);

    bool exception_thrown = false;

    try {
        double_upper.as<std::int64_t>();
    } catch (const std::out_of_range&) {
        exception_thrown = true;
    }

    assert(exception_thrown);

    // float -> int64_t boundaries
    value float_min(-9223372036854775808.0f);

    assert(float_min.as<std::int64_t>() ==
           std::numeric_limits<std::int64_t>::min());

    value float_upper(9223372036854775808.0f);

    exception_thrown = false;

    try {
        float_upper.as<std::int64_t>();
    } catch (const std::out_of_range&) {
        exception_thrown = true;
    }

    assert(exception_thrown);

    // NaN
    exception_thrown = false;

    try {
        value(std::numeric_limits<double>::quiet_NaN())
            .as<std::int64_t>();
    } catch (const std::out_of_range&) {
        exception_thrown = true;
    }

    assert(exception_thrown);

    // Positive infinity
    exception_thrown = false;

    try {
        value(std::numeric_limits<double>::infinity())
            .as<std::int64_t>();
    } catch (const std::out_of_range&) {
        exception_thrown = true;
    }

    assert(exception_thrown);

    // Negative infinity
    exception_thrown = false;

    try {
        value(-std::numeric_limits<double>::infinity())
            .as<std::int64_t>();
    } catch (const std::out_of_range&) {
        exception_thrown = true;
    }

    assert(exception_thrown);

    // Unsupported conversion
    exception_thrown = false;

    try {
        value("42").as<std::int64_t>();
    } catch (const std::invalid_argument&) {
        exception_thrown = true;
    }

    assert(exception_thrown);
}

void test_float_conversion() {
    using namespace ddl;

    // nullptr and bool
    assert(value(nullptr).as<float>() == 0.0f);
    assert(value(true).as<float>() == 1.0f);
    assert(value(false).as<float>() == 0.0f);

    // Integers
    assert(value(std::int32_t{42}).as<float>() == 42.0f);
    assert(value(std::int64_t{123456}).as<float>() == 123456.0f);

    // int64_t -> float precision loss
    std::int64_t large_int64 = 16777217LL;

    assert(value(large_int64).as<float>() ==
           static_cast<float>(large_int64));

    assert(value(large_int64).as<float>() !=
           static_cast<float>(16777217LL + 1));

    // float
    assert(value(3.14f).as<float>() == 3.14f);

    // double -> float precision loss
    assert(value(3.14).as<float>() == static_cast<float>(3.14));

    // char
    assert(value('A').as<float>() == static_cast<float>('A'));

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
    bool exception_thrown = false;

    try {
        value(std::numeric_limits<double>::max()).as<float>();
    } catch (const std::out_of_range&) {
        exception_thrown = true;
    }

    assert(exception_thrown);

    // Negative overflow
    exception_thrown = false;

    try {
        value(-std::numeric_limits<double>::max()).as<float>();
    } catch (const std::out_of_range&) {
        exception_thrown = true;
    }

    assert(exception_thrown);

    // Unsupported conversion
    exception_thrown = false;

    try {
        value("3.14").as<float>();
    } catch (const std::invalid_argument&) {
        exception_thrown = true;
    }

    assert(exception_thrown);
}

void test_double_conversion() {
    using namespace ddl;

    // nullptr and bool
    assert(value(nullptr).as<double>() == 0.0);
    assert(value(true).as<double>() == 1.0);
    assert(value(false).as<double>() == 0.0);

    // Integers
    assert(value(std::int32_t{42}).as<double>() == 42.0);
    assert(value(std::int64_t{123456789}).as<double>() == 123456789.0);

    // int64_t -> double precision loss
    std::int64_t large_int64 = 9007199254740993LL;

    assert(value(large_int64).as<double>() ==
           static_cast<double>(large_int64));

    // float -> double
    assert(value(3.14f).as<double>() == static_cast<double>(3.14f));

    // double
    assert(value(3.14).as<double>() == 3.14);

    // char
    assert(value('A').as<double>() == static_cast<double>('A'));

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
    bool exception_thrown = false;

    try {
        value("3.14").as<double>();
    } catch (const std::invalid_argument&) {
        exception_thrown = true;
    }

    assert(exception_thrown);
}

void test_char_conversion() {
    using namespace ddl;

    assert(value('A').as<char>() == 'A');
    assert(value('z').as<char>() == 'z');
    assert(value('\0').as<char>() == '\0');

    bool exception_thrown = false;

    try {
        value(65).as<char>();
    } catch (const std::invalid_argument&) {
        exception_thrown = true;
    }

    assert(exception_thrown);
}

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


// Main conversion test runner
void test_value_conversions() {
    test_bool_conversion();
    test_int32_conversion();
    test_int64_conversion();
    test_float_conversion();
    test_double_conversion();
    test_char_conversion();
    test_string_conversion();
}

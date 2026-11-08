#include <iostream>

// ============================================================
// Test Function Declarations
// ============================================================

void test_value_conversions();
void test_value_constructors();
void test_value_type_checker();
void test_value_printer();
void test_value_copy_move();
void test_value_arrays();
void test_value_objects();
void test_value_references();

// ============================================================
// Main Test Runner
// ============================================================

int main() {

    // --------------------------------------------------------
    // Value Tests
    // --------------------------------------------------------

    test_value_conversions();
    std::cout << "[PASS] Value conversions\n";

    test_value_constructors();
    std::cout << "[PASS] Value constructors\n";

    test_value_type_checker();
    std::cout << "[PASS] Value type checker\n";

    test_value_printer();
    std::cout << "[PASS] Value printer\n";

    test_value_copy_move();
    std::cout << "[PASS] Value copy and move semantics\n";

    // --------------------------------------------------------
    // Container Tests
    // --------------------------------------------------------

    test_value_arrays();
    std::cout << "[PASS] Value arrays\n";

    test_value_objects();
    std::cout << "[PASS] Value objects\n";

    // --------------------------------------------------------
    // Reference Tests
    // --------------------------------------------------------

    test_value_references();
    std::cout << "[PASS] Value references\n";

    // --------------------------------------------------------
    // Test Summary
    // --------------------------------------------------------

    std::cout << "\nAll tests passed successfully!\n";

    return 0;
}

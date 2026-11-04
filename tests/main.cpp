#include <iostream>

// Test function declarations
void test_value_conversions();
void test_value_constructors();
void test_value_type_checker();
void test_value_printer();
void test_value_copy_move();
void test_value_arrays();
void test_value_objects();

int main() {
    test_value_conversions();
    std::cout << "All value conversion tests passed!\n";
    test_value_constructors();
    std::cout << "All value constructor tests passed!\n";
    test_value_type_checker();
    std::cout << "All value type checker tests passed!\n";
    test_value_printer();
    std::cout << "All value printer tests passed!\n";
    test_value_copy_move();
    std::cout << "All value copy move tests passed!\n";
    test_value_arrays();
    std::cout << "All value array tests passed!\n";
    test_value_objects();
    std::cout << "All value object tests passed!\n";

    std::cout << "\nAll tests passed!\n";

    return 0;
}
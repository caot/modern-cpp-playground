#include <iostream>
#include <string>
#include <vector>

// Function with a complex return type that can be simplified with decltype.

// The auto keyword in the function signature is a placeholder, and the actual return type is specified after the
// parameter list using -> decltype(...). This allows the function to handle various combinations of types, such
// as int and double, and correctly deduce the return type for each.
template <typename T, typename U>
auto add_and_get_result(T a, U b) -> decltype(a + b) {  // declared type
    return a + b;
}

int main() {
    // --- Basic `decltype` usage ---
    int x = 5;
    const std::string s = "hello";

    // `decltype(x)` determines the type of `x`, which is `int`.
    decltype(x) y = 10;
    std::cout << "The type of `y` is `int`, its value is: " << y << std::endl;

    // `decltype(s)` determines the type of `s`, which is `const std::string`.
    decltype(s) t = "world";
    std::cout << "The type of `t` is `const std::string`, its value is: " << t << std::endl;

    // --- `decltype` with expressions ---
    int a = 10, b = 20;
    // `decltype(a + b)` determines the type of the result of the addition, which is `int`.
    decltype(a + b) sum = a + b;
    std::cout << "The type of `sum` is `int`, its value is: " << sum << std::endl;

    double d = 3.5;
    // `decltype(a + d)` determines the type of the result, which is `double` due to promotion.
    decltype(a + d) result = a + d;
    std::cout << "The type of `result` is `double`, its value is: " << result << std::endl;

    // --- `decltype` with functions (Trailing return type) ---
    int int_val = 5;
    double double_val = 2.5;
    std::string str_val = " C++";

    // The compiler deduces the return type as `int`.
    auto int_sum = add_and_get_result(int_val, 10);
    std::cout << "Sum of int: " << int_sum << " (type: " << typeid(int_sum).name() << ")" << std::endl;

    // The compiler deduces the return type as `double`.
    auto double_sum = add_and_get_result(int_val, double_val);
    std::cout << "Sum of int and double: " << double_sum << " (type: " << typeid(double_sum).name() << ")" << std::endl;

    // The compiler deduces the return type as `std::string`.
    auto string_concat = add_and_get_result(str_val, "!");
    std::cout << "Concatenated string: " << string_concat << " (type: " << typeid(string_concat).name() << ")" << std::endl;

    return 0;
}

/*
 * The "decl" in decltype is an abbreviation for "declared type."
 *
 * decltype, a keyword that determines the type of an expression at compile time.
 *
 * The decltype keyword is a type specifier that yields the type of a given expression. It's especially useful in generic programming and with complex return types for functions. It doesn't evaluate the expression; it only analyzes its type.
 */

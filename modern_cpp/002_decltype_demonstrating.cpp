#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <type_traits>
#include <functional>
#include <memory>
#include <typeinfo>
#include <cxxabi.h>

// Helper function to print type names (GCC/Clang specific)
template<typename T>
std::string getTypeName() {
    const char* name = typeid(T).name();
    int status = 0;
    char* demangled = abi::__cxa_demangle(name, 0, 0, &status);
    std::string result = (status == 0) ? demangled : name;
    free(demangled);
    return result;
}

// Macro to print type information
#define PRINT_TYPE(expr) \
    std::cout << #expr << " -> " << getTypeName<decltype(expr)>() << std::endl

// ========================================
// 1. BASIC DECLTYPE USAGE
// ========================================

void demonstrateBasicDecltype() {
    std::cout << "\n=== BASIC DECLTYPE USAGE ===" << std::endl;

    int x = 42;
    double y = 3.14;
    const int z = 100;
    int& ref = x;
    const int& const_ref = z;

    // Basic type deduction
    PRINT_TYPE(x);        // int
    PRINT_TYPE(y);        // double
    PRINT_TYPE(z);        // int (const is part of the type)
    PRINT_TYPE(ref);      // int& (reference type)
    PRINT_TYPE(const_ref); // const int&

    // Using decltype to declare variables
    decltype(x) var1 = 50;           // int var1 = 50;
    decltype(y) var2 = 2.71;         // double var2 = 2.71;
    decltype(z) var3 = 200;          // const int var3 = 200; (const!)
    decltype(ref) var4 = x;          // int& var4 = x; (reference!)

    std::cout << "var1: " << var1 << " (type: " << getTypeName<decltype(var1)>() << ")" << std::endl;
    std::cout << "var2: " << var2 << " (type: " << getTypeName<decltype(var2)>() << ")" << std::endl;
    std::cout << "var3: " << var3 << " (type: " << getTypeName<decltype(var3)>() << ")" << std::endl;
    std::cout << "var4: " << var4 << " (type: " << getTypeName<decltype(var4)>() << ")" << std::endl;

    // Decltype with expressions
    PRINT_TYPE(x + y);    // double (result of int + double)
    PRINT_TYPE(x * 2);    // int
    PRINT_TYPE(++x);      // int& (pre-increment returns reference)
    PRINT_TYPE(x++);      // int (post-increment returns value)
}

// ========================================
// 2. DECLTYPE VS AUTO
// ========================================

void demonstrateDecltypeVsAuto() {
    std::cout << "\n=== DECLTYPE VS AUTO ===" << std::endl;

    int x = 42;
    const int& ref = x;

    // auto strips references and const
    auto auto_var = ref;        // int (not const int&)
    decltype(ref) decltype_var = ref;  // const int&

    PRINT_TYPE(auto_var);       // int
    PRINT_TYPE(decltype_var);   // const int&

    std::cout << "Original ref: " << ref << std::endl;

    auto_var = 100;      // OK - auto_var is just int
    // decltype_var = 100;  // ERROR - decltype_var is const int&

    std::cout << "auto_var after change: " << auto_var << std::endl;
    std::cout << "decltype_var (unchanged): " << decltype_var << std::endl;

    // Array examples
    int arr[5] = {1, 2, 3, 4, 5};

    auto auto_arr = arr;           // int* (array decays to pointer)
    decltype(arr) decltype_arr = {6, 7, 8, 9, 10};  // int[5] (preserves array type)

    PRINT_TYPE(auto_arr);          // int*
    PRINT_TYPE(decltype_arr);      // int [5]

    std::cout << "Size of auto_arr: " << sizeof(auto_arr) << " bytes" << std::endl;     // Size of pointer
    std::cout << "Size of decltype_arr: " << sizeof(decltype_arr) << " bytes" << std::endl; // Size of array
}

// ========================================
// 3. DECLTYPE WITH CONTAINERS
// ========================================

void demonstrateDecltypeWithContainers() {
    std::cout << "\n=== DECLTYPE WITH CONTAINERS ===" << std::endl;

    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::map<std::string, double> scores = {{"Alice", 95.5}, {"Bob", 87.2}};

    // Container element types
    PRINT_TYPE(vec[0]);           // int&
    PRINT_TYPE(vec.front());      // int&
    PRINT_TYPE(vec.size());       // std::vector<int>::size_type (usually size_t)

    // Map element types
    PRINT_TYPE(scores["Alice"]);  // double&
    PRINT_TYPE(scores.begin()->first);   // const std::string&
    PRINT_TYPE(scores.begin()->second);  // double&

    // Using decltype to declare compatible variables
    decltype(vec)::value_type element = 42;  // int element = 42;
    decltype(vec.size()) index = 2;          // size_t index = 2;
    decltype(vec[0]) ref_element = vec[1];   // int& ref_element = vec[1];

    std::cout << "element: " << element << std::endl;
    std::cout << "index: " << index << std::endl;
    std::cout << "ref_element: " << ref_element << std::endl;

    // Iterator types
    decltype(vec.begin()) it1 = vec.begin();
    decltype(scores.find("Alice")) it2 = scores.find("Bob");

    PRINT_TYPE(it1);  // std::vector<int>::iterator
    PRINT_TYPE(it2);  // std::map<std::string, double>::iterator

    if (it2 != scores.end()) {
        std::cout << "Found " << it2->first << " with score " << it2->second << std::endl;
    }
}

// ========================================
// 4. DECLTYPE IN FUNCTION TEMPLATES
// ========================================

// Template function using decltype for return type deduction
template<typename T, typename U>
auto multiply(T a, U b) -> decltype(a * b) {
    std::cout << "Multiplying " << getTypeName<T>() << " * " << getTypeName<U>()
              << " = " << getTypeName<decltype(a * b)>() << std::endl;
    return a * b;
}

// C++14 style - auto return type deduction
template<typename Container>
auto getFirstElement(Container& container) -> decltype(container[0]) {
    if (!container.empty()) {
        return container[0];
    }
    throw std::runtime_error("Container is empty");
}

void processArgument(int& x) {
    std::cout << "Processing lvalue reference: " << x << std::endl;
    ++x;
}

void processArgument(const int& x) {
    std::cout << "Processing const reference: " << x << std::endl;
}

void processArgument(int&& x) {
    std::cout << "Processing rvalue reference: " << x << std::endl;
}

// Perfect forwarding with decltype
template<typename T>
void perfectForward(T&& arg) {
    using ArgType = decltype(arg);
    std::cout << "Perfect forwarding: " << getTypeName<ArgType>() << std::endl;

    // Forward to another function
    processArgument(std::forward<T>(arg));
}

void demonstrateDecltypeInTemplates() {
    std::cout << "\n=== DECLTYPE IN TEMPLATES ===" << std::endl;

    // Template function calls
    auto result1 = multiply(5, 2.5);        // int * double = double
    auto result2 = multiply(3.0f, 7);       // float * int = float
    auto result3 = multiply(2LL, 4);        // long long * int = long long

    std::cout << "Results: " << result1 << ", " << result2 << ", " << result3 << std::endl;

    // Container element access
    std::vector<std::string> names = {"Alice", "Bob", "Charlie"};
    try {
        decltype(getFirstElement(names)) first = getFirstElement(names);
        std::cout << "First element: " << first << " (type: "
                  << getTypeName<decltype(first)>() << ")" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    // Perfect forwarding examples
    int value = 42;
    const int const_value = 100;

    std::cout << "\nPerfect forwarding examples:" << std::endl;
    perfectForward(value);              // lvalue
    perfectForward(const_value);        // const lvalue
    perfectForward(200);                // rvalue
    perfectForward(std::move(value));   // rvalue (moved)

    std::cout << "value after forwarding: " << value << std::endl;
}

// ========================================
// 5. DECLTYPE WITH LAMBDAS AND FUNCTION OBJECTS
// ========================================

void demonstrateDecltypeWithLambdas() {
    std::cout << "\n=== DECLTYPE WITH LAMBDAS ===" << std::endl;

    // Simple lambda
    auto lambda1 = [](int x) { return x * 2; };
    auto lambda2 = [](double x, double y) -> double { return x + y; };

    // Decltype with lambda calls
    PRINT_TYPE(lambda1(5));         // int
    PRINT_TYPE(lambda2(1.5, 2.5));  // double

    // Using decltype to store lambda results
    decltype(lambda1(10)) result1 = lambda1(15);  // int result1 = lambda1(15);
    decltype(lambda2(1.0, 2.0)) result2 = lambda2(3.5, 4.5);  // double result2 = ...;

    std::cout << "Lambda results: " << result1 << ", " << result2 << std::endl;

    // Generic lambda (C++14)
    auto generic_lambda = [](auto a, auto b) { return a + b; };

    PRINT_TYPE(generic_lambda(5, 10));      // int
    PRINT_TYPE(generic_lambda(1.5, 2.7));   // double
    PRINT_TYPE(generic_lambda(std::string("Hello"), std::string(" World")));  // std::string

    // Function object
    std::function<int(int, int)> func_obj = [](int a, int b) { return a * b; };
    PRINT_TYPE(func_obj(3, 4));  // int

    decltype(func_obj(1, 1)) func_result = func_obj(6, 7);
    std::cout << "Function object result: " << func_result << std::endl;
}

// ========================================
// 6. ADVANCED DECLTYPE USAGE
// ========================================

// SFINAE (Substitution Failure Is Not An Error) with decltype
template<typename T>
class HasSizeMethod {
private:
    template<typename U>
    static auto test(U* u) -> decltype(u->size(), std::true_type{});

    template<typename>
    static std::false_type test(...);

public:
    static constexpr bool value = decltype(test<T>(nullptr))::value;
};

// Template specialization using decltype
template<typename T>
typename std::enable_if<HasSizeMethod<T>::value, size_t>::type
getSize(const T& container) {
    return container.size();
}

template<typename T>
typename std::enable_if<!HasSizeMethod<T>::value, size_t>::type
getSize(const T&) {
    return 0;  // Default size for types without size() method
}

// Decltype with member function pointers
class Calculator {
public:
    int add(int a, int b) { return a + b; }
    double multiply(double a, double b) { return a * b; }

    static int staticAdd(int a, int b) { return a + b; }
};

void demonstrateAdvancedDecltype() {
    std::cout << "\n=== ADVANCED DECLTYPE USAGE ===" << std::endl;

    // SFINAE examples
    std::vector<int> vec = {1, 2, 3};
    int regular_int = 42;

    std::cout << "Vector has size method: " << HasSizeMethod<std::vector<int>>::value << std::endl;
    std::cout << "int has size method: " << HasSizeMethod<int>::value << std::endl;

    std::cout << "Vector size: " << getSize(vec) << std::endl;
    std::cout << "Int size: " << getSize(regular_int) << std::endl;

    // Member function pointers with decltype
    Calculator calc;

    // Deduce member function pointer types
    decltype(&Calculator::add) add_ptr = &Calculator::add;
    decltype(&Calculator::multiply) mult_ptr = &Calculator::multiply;
    decltype(&Calculator::staticAdd) static_add_ptr = &Calculator::staticAdd;

    PRINT_TYPE(add_ptr);         // int (Calculator::*)(int, int)
    PRINT_TYPE(mult_ptr);        // double (Calculator::*)(double, double)
    PRINT_TYPE(static_add_ptr);  // int (*)(int, int)

    // Use the function pointers
    int add_result = (calc.*add_ptr)(10, 20);
    double mult_result = (calc.*mult_ptr)(3.5, 4.2);
    int static_result = static_add_ptr(5, 7);

    std::cout << "Member function results: " << add_result << ", "
              << mult_result << ", " << static_result << std::endl;
}

// ========================================
// 7. DECLTYPE WITH EXPRESSIONS AND OPERATORS
// ========================================

void demonstrateDecltypeWithExpressions() {
    std::cout << "\n=== DECLTYPE WITH EXPRESSIONS ===" << std::endl;

    int a = 10, b = 20;
    double x = 1.5, y = 2.7;

    // Arithmetic expressions
    PRINT_TYPE(a + b);      // int
    PRINT_TYPE(a + x);      // double (int promoted to double)
    PRINT_TYPE(x * y);      // double
    PRINT_TYPE(a / b);      // int (integer division)
    PRINT_TYPE(x / y);      // double

    // Comparison expressions
    PRINT_TYPE(a < b);      // bool
    PRINT_TYPE(x >= y);     // bool
    PRINT_TYPE(a == b);     // bool

    // Logical expressions
    PRINT_TYPE(a && b);     // bool
    PRINT_TYPE(!a);         // bool

    // Conditional operator
    PRINT_TYPE(true ? a : b);   // int
    PRINT_TYPE(true ? a : x);   // double (common type of int and double)
    PRINT_TYPE(false ? x : y);  // double

    // Assignment expressions
    PRINT_TYPE(a = b);      // int& (assignment returns reference to lvalue)
    PRINT_TYPE(a += 5);     // int&
    PRINT_TYPE(++a);        // int& (pre-increment)
    PRINT_TYPE(a++);        // int (post-increment returns copy)

    // Complex expressions
    PRINT_TYPE((a + b) * (x - y));  // double

    // Using decltype with complex expressions
    decltype(a + x) result = a + x;  // double result
    decltype(a < b ? a : b) min_val = (a < b ? a : b);  // std::min(a, b);  // int min_val

    std::cout << "Complex expression result: " << result << std::endl;
    std::cout << "Min value: " << min_val << std::endl;

    // Array subscript expressions
    int arr[5] = {1, 2, 3, 4, 5};
    PRINT_TYPE(arr[0]);     // int& (subscript returns reference)
    PRINT_TYPE(arr[2]);     // int&

    decltype(arr[0]) element_ref = arr[3];  // int& element_ref = arr[3];
    element_ref = 100;  // Modifies arr[3]
    std::cout << "Modified array element: " << arr[3] << std::endl;
}

// ========================================
// 8. PRACTICAL EXAMPLES AND USE CASES
// ========================================

// Generic container printer using decltype
template<typename Container>
void printContainer(const Container& container) {
    std::cout << "Container contents: ";

    // Use decltype to get the proper iterator type
    for (decltype(container.begin()) it = container.begin();
         it != container.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // Element type information
    using ElementType = decltype(*container.begin());
    std::cout << "Element type: " << getTypeName<ElementType>() << std::endl;
}

// Generic algorithm using decltype
template<typename Container, typename Predicate>
auto countIf(const Container& container, Predicate pred)
    -> decltype(container.size()) {

    decltype(container.size()) count = 0;
    for (const auto& element : container) {
        if (pred(element)) {
            ++count;
        }
    }
    return count;
}

void demonstratePracticalExamples() {
    std::cout << "\n=== PRACTICAL EXAMPLES ===" << std::endl;

    // Different container types
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    std::vector<std::string> words = {"hello", "world", "cpp", "decltype"};

    printContainer(numbers);
    printContainer(words);

    // Generic counting
    auto even_count = countIf(numbers, [](int x) { return x % 2 == 0; });
    auto long_words = countIf(words, [](const std::string& s) { return s.length() > 3; });

    std::cout << "Even numbers count: " << even_count << std::endl;
    std::cout << "Long words count: " << long_words << std::endl;

    // Type matching for generic programming
    std::map<std::string, int> word_counts = {{"hello", 5}, {"world", 5}, {"cpp", 3}};

    // Extract key and value types using decltype
    // using KeyType = decltype(word_counts.begin()->first);  // error: no match for ‘operator=’ (operand types are ‘KeyType’ {aka ‘const std::__cxx11::basic_string<char>’} and ‘const std::__cxx11::basic_string<char>’)
    using KeyType = std::remove_const_t<std::remove_reference_t<decltype(word_counts.begin()->first)>>;  // Result: std::string (plain value type)
    using ValueType = decltype(word_counts.begin()->second);

    std::cout << "Map key type: " << getTypeName<KeyType>() << std::endl;
    std::cout << "Map value type: " << getTypeName<ValueType>() << std::endl;

    // Use these types to create compatible variables
    KeyType longest_word;
    ValueType max_count = 0;

    for (const auto& pair : word_counts) {
        if (pair.second > max_count) {
            max_count = pair.second;
            longest_word = pair.first;
        }
    }

    std::cout << "Longest word: '" << longest_word << "' with " << max_count << " characters" << std::endl;
}

int main() {
    std::cout << "=== C++ DECLTYPE COMPREHENSIVE EXAMPLES ===" << std::endl;

    demonstrateBasicDecltype();
    demonstrateDecltypeVsAuto();
    demonstrateDecltypeWithContainers();
    demonstrateDecltypeInTemplates();
    demonstrateDecltypeWithLambdas();
    demonstrateAdvancedDecltype();
    demonstrateDecltypeWithExpressions();
    demonstratePracticalExamples();

    std::cout << "\n🎉 All decltype demonstrations completed!" << std::endl;
    std::cout << "decltype is a powerful tool for type deduction and generic programming!" << std::endl;

    return 0;
}

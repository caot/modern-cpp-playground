/*
auto, nullptr, and range-for

Compilation:

# Requires C++11 or later
g++ -std=c++17 -o modern_cpp modern_cpp.cpp

# Run the example
./modern_cpp
 */

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <memory>
#include <algorithm>
#include <functional>

// ========================================
// 1. NULLPTR EXAMPLES
// ========================================

class Logger {
public:
    void log(const std::string& message) {
        std::cout << "[LOG] " << message << std::endl;
    }
};

// Function that takes a pointer parameter
void processData(int* data, Logger* logger = nullptr) {
    // Modern nullptr check (C++11+)
    if (data == nullptr) {
        std::cout << "Error: data pointer is null!" << std::endl;
        return;
    }

    std::cout << "Processing data: " << *data << std::endl;

    // Optional logger - safe nullptr check
    if (logger != nullptr) {
        logger->log("Data processed successfully");
    }
}

// Smart pointer example with nullptr
std::unique_ptr<Logger> createLogger(bool enable) {
    if (enable) {
        return std::make_unique<Logger>();
    }
    return nullptr; // Return null smart pointer
}

// Function overloading - nullptr vs 0 vs NULL distinction
void overloadExample(int value) {
    std::cout << "Called with int: " << value << std::endl;
}

void overloadExample(void* ptr) {
    std::cout << "Called with pointer: " << (ptr ? "valid" : "null") << std::endl;
}

// ========================================
// 2. AUTO EXAMPLES
// ========================================

void demonstrateAuto() {
    std::cout << "\n=== AUTO KEYWORD EXAMPLES ===" << std::endl;

    // Basic auto usage
    auto x = 42;                    // int
    auto y = 3.14;                  // double
    auto z = "Hello";               // const char*
    auto str = std::string("World"); // std::string

    std::cout << "x (int): " << x << std::endl;
    std::cout << "y (double): " << y << std::endl;
    std::cout << "z (const char*): " << z << std::endl;
    std::cout << "str (string): " << str << std::endl;

    // Auto with containers
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    auto it = numbers.begin();      // std::vector<int>::iterator
    auto size = numbers.size();     // std::vector<int>::size_type

    std::cout << "First element: " << *it << std::endl;
    std::cout << "Vector size: " << size << std::endl;

    // Auto with complex types
    std::map<std::string, std::vector<int>> complexMap;
    complexMap["numbers"] = {10, 20, 30};

    // Without auto: std::map<std::string, std::vector<int>>::iterator
    auto mapIt = complexMap.find("numbers");
    if (mapIt != complexMap.end()) {
        std::cout << "Found key 'numbers' with " << mapIt->second.size() << " elements" << std::endl;
    }

    // Auto with lambdas
    auto lambda = [](int a, int b) -> int {
        return a + b;
    };
    std::cout << "Lambda result: " << lambda(5, 7) << std::endl;

    // Auto with function pointers
    // auto funcPtr = &overloadExample;  // error: unable to deduce ‘auto’ from ‘& overloadExample’
    // funcPtr(nullptr); // This would call the pointer version

    // Auto with function pointers - need to specify which overload
    auto funcPtr1 = static_cast<void(*)(int)>(&overloadExample);
    auto funcPtr2 = static_cast<void(*)(void*)>(&overloadExample);

    // Auto with references and const
    int value = 100;
    auto& ref = value;          // int&
    const auto& constRef = value; // const int&
    auto* ptr = &value;         // int*

    ref = 200; // Modifies original value
    std::cout << "Modified value through reference: " << value << std::endl;
}

// Auto in function parameters (C++14 generic lambdas concept)
template<typename T>
void printType(T&& value) {
    std::cout << "Value: " << value << std::endl;
}

// Auto return type (C++14)
auto calculateSum(const std::vector<int>& vec) -> int // decltype(vec[0])
{
    auto sum = 0; // int
    for (const auto& element : vec) {
        sum += element;
    }
    return sum;  // warning: reference to local variable ‘sum’ returned
}

// Alternative version showing decltype usage correctly
auto calculateAverage(const std::vector<double>& vec) -> std::remove_reference_t<decltype(vec[0])> {
    if (vec.empty()) return 0.0;

    auto sum = 0.0;
    for (const auto& element : vec) {
        sum += element;
    }
    return sum / vec.size(); // Returns double (same as vec[0] type)
}

// ========================================
// 3. RANGE-BASED FOR LOOP EXAMPLES
// ========================================

void demonstrateRangeFor() {
    std::cout << "\n=== RANGE-BASED FOR LOOP EXAMPLES ===" << std::endl;

    // Basic range-for with vector
    std::vector<int> numbers = {1, 2, 3, 4, 5};

    std::cout << "Numbers (by value): ";
    for (auto num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    // Range-for with reference (for modification)
    std::cout << "Doubling numbers: ";
    for (auto& num : numbers) {
        num *= 2;
        std::cout << num << " ";
    }
    std::cout << std::endl;

    // Range-for with const reference (efficient, no copying)
    std::cout << "Numbers (const ref): ";
    for (const auto& num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    // Range-for with strings
    std::vector<std::string> names = {"Alice", "Bob", "Charlie"};
    std::cout << "Names: ";
    for (const auto& name : names) {
        std::cout << name << " ";
    }
    std::cout << std::endl;

    // Range-for with map
    std::map<std::string, int> scores = {
        {"Alice", 95},
        {"Bob", 87},
        {"Charlie", 92}
    };

    std::cout << "Scores:" << std::endl;
    for (const auto& pair : scores) {
        std::cout << "  " << pair.first << ": " << pair.second << std::endl;
    }

    // Structured bindings (C++17) - even cleaner
    std::cout << "Scores (C++17 structured bindings):" << std::endl;
    for (const auto& [name, score] : scores) {
        std::cout << "  " << name << ": " << score << std::endl;
    }

    // Range-for with arrays
    int arr[] = {10, 20, 30, 40, 50};
    std::cout << "Array elements: ";
    for (const auto& element : arr) {
        std::cout << element << " ";
    }
    std::cout << std::endl;

    // Range-for with string (character iteration)
    std::string text = "Hello";
    std::cout << "Characters: ";
    for (auto ch : text) {
        std::cout << ch << " ";
    }
    std::cout << std::endl;
}

// Custom class that works with range-based for
class NumberRange {
private:
    int m_start, m_end;

public:
    NumberRange(int s, int e) : m_start(s), m_end(e) {}

    // Iterator class
    class Iterator {
    private:
        int current;
    public:
        Iterator(int val) : current(val) {}

        int operator*() const { return current; }
        Iterator& operator++() { ++current; return *this; }
        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }
    };

    [[nodiscard]] Iterator begin() const { return {m_start}; }
    [[nodiscard]] Iterator end() const { return {m_end}; }
};

// ========================================
// 4. COMBINING ALL FEATURES
// ========================================

class DataProcessor {
private:
    std::vector<std::unique_ptr<Logger>> loggers;

public:
    void addLogger(std::unique_ptr<Logger> logger) {
        if (logger != nullptr) { // nullptr check
            loggers.push_back(std::move(logger));
        }
    }

    void processNumbers(const std::vector<int>& numbers) {
        // Auto with algorithm
        auto maxElement = std::max_element(numbers.begin(), numbers.end());
        if (maxElement != numbers.end()) {
            // Range-based for with loggers
            for (const auto& logger : loggers) {
                if (logger != nullptr) { // Smart pointer nullptr check
                    logger->log("Max element: " + std::to_string(*maxElement));
                }
            }
        }

        // Auto with lambda and range-for
        auto evenCount = 0;
        for (const auto& num : numbers) {
            if (num % 2 == 0) {
                ++evenCount;
            }
        }

        // Log results
        for (const auto& logger : loggers) {
            if (logger) { // Implicit nullptr check (smart pointers)
                logger->log("Even numbers found: " + std::to_string(evenCount));
            }
        }
    }
};

// Advanced example: auto with trailing return type
template<typename Container>
auto findElement(const Container& container, const typename Container::value_type& value)
    -> decltype(container.begin()) {

    for (auto it = container.begin(); it != container.end(); ++it) {
        if (*it == value) {
            return it;
        }
    }
    return container.end();
}

int main() {
    std::cout << "=== MODERN C++ FEATURES DEMONSTRATION ===" << std::endl;

    // ========================================
    // NULLPTR EXAMPLES
    // ========================================
    std::cout << "\n=== NULLPTR EXAMPLES ===" << std::endl;

    int data = 42;
    Logger logger;

    // Valid data and logger
    processData(&data, &logger);

    // Valid data, no logger
    processData(&data, nullptr);

    // Invalid data
    processData(nullptr, &logger);

    // Smart pointer with nullptr
    auto smartLogger = createLogger(true);
    if (smartLogger != nullptr) {
        smartLogger->log("Smart pointer logger created");
    }

    auto nullLogger = createLogger(false);
    if (nullLogger == nullptr) {
        std::cout << "Smart pointer is null (as expected)" << std::endl;
    }

    // Demonstrate nullptr vs 0 vs NULL
    std::cout << "\nFunction overloading with nullptr:" << std::endl;
    overloadExample(0);         // Calls int version
    overloadExample(nullptr);   // Calls pointer version
    // overloadExample(NULL);   // Ambiguous in some cases

    // ========================================
    // AUTO EXAMPLES
    // ========================================
    demonstrateAuto();

    // Auto with algorithms
    std::vector<int> testVec = {5, 2, 8, 1, 9};
    auto sum = calculateSum(testVec);
    std::cout << "Sum of vector: " << sum << std::endl;

    // ========================================
    // RANGE-BASED FOR EXAMPLES
    // ========================================
    demonstrateRangeFor();

    // Custom range class
    std::cout << "\nCustom range class:" << std::endl;
    NumberRange range(1, 6);
    for (auto num : range) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    // ========================================
    // COMBINED EXAMPLE
    // ========================================
    std::cout << "\n=== COMBINED FEATURES EXAMPLE ===" << std::endl;

    DataProcessor processor;

    // Add loggers using smart pointers and nullptr
    processor.addLogger(std::make_unique<Logger>());
    processor.addLogger(nullptr); // This won't be added
    processor.addLogger(std::make_unique<Logger>());

    // Process data using all modern features
    std::vector<int> dataSet = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    processor.processNumbers(dataSet);

    // Advanced template example
    auto foundIt = findElement(dataSet, 5);
    if (foundIt != dataSet.end()) {
        std::cout << "Found element 5 at position: "
                  << std::distance(dataSet.cbegin(), foundIt) << std::endl;  // Both are const_iterator now
    }

    std::cout << "\n=== DEMONSTRATION COMPLETE ===" << std::endl;
    return 0;
}

/*
This comprehensive example demonstrates the three key modern C++ features with practical use cases:
1. nullptr Examples:

Type Safety: nullptr is type-safe unlike NULL or 0
Function Overloading: Shows how nullptr resolves ambiguity
Smart Pointers: Safe usage with std::unique_ptr
Parameter Checking: Proper null pointer validation
Optional Parameters: Using nullptr as default values

2. auto Examples:

Type Deduction: Automatic type inference for variables
Complex Types: Simplifies iterator and container declarations
Lambdas: Auto with lambda expressions
References & Pointers: auto&, const auto&, auto*
Function Returns: Auto return types (C++14)
Template Functions: Trailing return types with decltype

3. Range-based for Examples:

Basic Iteration: Simple container traversal
References: auto& for modification, const auto& for efficiency
Different Containers: vectors, maps, arrays, strings
Structured Bindings: C++17 feature for cleaner map iteration
Custom Classes: Implementing begin()/end() for custom types

Key Benefits:
nullptr:

Type safety (won't accidentally convert to int)
Clear intent in code
Better function overload resolution
Works seamlessly with smart pointers

auto:

Reduces verbosity for complex types
Prevents type mismatches
Makes code more maintainable
Especially useful with templates and STL

Range-based for:

Cleaner, more readable code
Less error-prone than traditional for loops
Works with any container that has begin()/end()
More expressive intent

Compilation:

# Requires C++11 or later
g++ -std=c++17 -o modern_cpp modern_cpp.cpp

# Run the example
./modern_cpp

This code demonstrates real-world usage patterns and best practices for these essential modern C++ features that make code safer, more readable, and more maintainable.
 */
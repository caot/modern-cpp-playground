#include <iostream>
#include <vector>
#include <chrono>
#include <string>

// A class with expensive copy and cheap move
class BigData {
public:
    std::vector<int> data;

    // Constructor
    BigData(size_t size) : data(size, 42) {}

    // Copy constructor (expensive)
    BigData(const BigData& other) : data(other.data) {
        std::cout << "Copy constructor called\n";
    }

    // Move constructor (cheap)
    BigData(BigData&& other) noexcept : data(std::move(other.data)) {
        std::cout << "Move constructor called\n";
    }
};

// Generic function template
template<typename T>
void process(T obj) {
    // Simulate some processing
    int sum = 0;
    for (auto& val : obj.data) {
        sum += val;
    }
}

int main() {
    const size_t dataSize = 10'000'000;
    BigData original(dataSize);

    // Measure copy time
    auto startCopy = std::chrono::high_resolution_clock::now();
    process<BigData>(original);  // invokes copy
    auto endCopy = std::chrono::high_resolution_clock::now();
    auto durationCopy = std::chrono::duration_cast<std::chrono::milliseconds>(endCopy - startCopy).count();

    // Measure move time
    auto startMove = std::chrono::high_resolution_clock::now();
    process<BigData>(std::move(original));  // invokes move
    auto endMove = std::chrono::high_resolution_clock::now();
    auto durationMove = std::chrono::duration_cast<std::chrono::milliseconds>(endMove - startMove).count();

    std::cout << "\nCopy took: " << durationCopy << " ms\n";
    std::cout << "Move took: " << durationMove << " ms\n";

    return 0;
}

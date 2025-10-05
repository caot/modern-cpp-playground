#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <sstream>
#include <string>

int main() {
    std::string text = "hello world hello C++ world of programming C++ is great programming is fun. The quick brown fox jumps over the lazy dog. The dog is lazy, and the fox is quick. The end of the quick story!";

    // Map to store word frequencies
    std::map<std::string, int> wordCount;

    // Parse text and count words
    std::istringstream iss(text);
    std::string word;
    while (iss >> word) {
        wordCount[word]++;
    }

    // Convert map to vector of pairs for sorting
    std::vector<std::pair<std::string, int>> wordVec(wordCount.begin(), wordCount.end());

    // Sort by frequency (descending) using lambda
    std::sort(wordVec.begin(), wordVec.end(),
        [](const auto& a, const auto& b) {
            return a.second > b.second; // Sort by count descending
        });

    // Display results
    std::cout << "Word Frequency (sorted by count):\n";
    std::cout << "-----------------------------------\n";
    for (const auto& pair : wordVec) {
        std::cout << pair.first << ": " << pair.second << "\n";
    }

    return 0;
}

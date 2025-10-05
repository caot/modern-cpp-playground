#include <iostream>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>

int main() {
    std::string text = "hello world hello C++ world of programming C++ is great programming is fun. The quick brown fox jumps over the lazy dog. The dog is lazy, and the fox is quick. The end of the quick story!";

    // Step 1: Count word frequency using std::map
    std::map<std::string, int> wordCount;
    std::istringstream iss(text);
    std::string word;
    while (iss >> word) {
        std::ranges::transform(text, text.begin(),
        [](unsigned char c) { return std::tolower(c); });

        ++wordCount[word];
    }

    // Step 2: Move map to vector for sorting
    std::vector<std::pair<std::string, int>> sortedWords(wordCount.begin(), wordCount.end());

    // Step 3: Sort using lambda by frequency descending
    std::sort(sortedWords.begin(), sortedWords.end(),
              [](const auto& a, const auto& b) {
                  return a.second > b.second;
              });

    // Step 4: Print results
    for (const auto& [word, count] : sortedWords) {
        std::cout << word << ": " << count << '\n';
    }

    return 0;
}

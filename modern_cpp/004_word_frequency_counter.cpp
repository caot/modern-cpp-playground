#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
#include <cctype>

/**
 * @brief Cleans a word by converting it to lowercase and removing trailing punctuation.
 * * This ensures that "Word," "word," and "word!" are all counted as the same word.
 * * @param word The word string to clean.
 * @return std::string The cleaned word.
 */
std::string clean_word(const std::string& word) {
    std::string cleaned = word;

    // 1. Convert to lowercase
    std::transform(cleaned.begin(), cleaned.end(), cleaned.begin(),
                   [](unsigned char c){ return std::tolower(c); });

    // 2. Remove trailing punctuation (simple cleanup for demo)
    while (!cleaned.empty() && std::ispunct(cleaned.back())) {
        cleaned.pop_back();
    }

    return cleaned;
}

/**
 * @brief Parses text and counts word frequencies using a std::map.
 * * @param text The input string containing the text to analyze.
 * @return std::map<std::string, int> A map of words to their frequency counts.
 */
std::map<std::string, int> count_word_frequency(const std::string& text) {
    std::map<std::string, int> frequency_map;
    // Changed to std::istringstream (Input String Stream) as we are only reading from the text.
    std::istringstream ss(text);
    std::string word_token;

    // Use stringstream to easily split the text into tokens (words)
    while (ss >> word_token) {
        std::string cleaned = clean_word(word_token);

        // Only count non-empty strings (e.g., if a token was just punctuation)
        if (!cleaned.empty()) {
            frequency_map[cleaned]++;
        }
    }

    return frequency_map;
}

int main() {
    // Sample text for analysis
    const std::string sample_text =
        "hello world hello C++ world of programming C++ is great programming is fun. The quick brown fox jumps over the lazy dog. The dog is lazy, and the fox is quick. The end of the quick story!";

    std::cout << "--- Word Frequency Counter ---" << std::endl;
    std::cout << "Input Text: \"" << sample_text.substr(0, 70) << "...\"" << std::endl;

    // 1. Count word frequencies using std::map
    std::map<std::string, int> word_counts = count_word_frequency(sample_text);

    // 2. Prepare for sorting: Copy map elements into a vector of pairs
    // std::map is sorted by key (alphabetically), so we must use a vector
    // if we want to sort by value (frequency).
    using WordFreqPair = std::pair<std::string, int>;
    std::vector<WordFreqPair> sorted_counts(word_counts.begin(), word_counts.end());

    // 3. Sort the vector using std::sort and a lambda function
    std::sort(sorted_counts.begin(), sorted_counts.end(),
        // The lambda takes two pairs (a and b) and compares them by the 'int' value (second element).
        // It returns true if 'a' should come before 'b', sorting in descending order (most frequent first).
        [](const WordFreqPair& a, const WordFreqPair& b) {
            return a.second > b.second; // Compare frequencies (descending order)
        }
    );

    // 4. Output the results
    std::cout << "\n--- Results (Sorted by Frequency) ---" << std::endl;
    std::cout << "Frequency\tWord" << std::endl;
    std::cout << "--------------------" << std::endl;

    for (const auto& pair : sorted_counts) {
        std::cout << pair.second << "\t\t" << pair.first << std::endl;
    }

    return 0;
}

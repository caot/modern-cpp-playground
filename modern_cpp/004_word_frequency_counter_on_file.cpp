// how to run:
//   g++ -std=c++20 -o 004_word_frequency_counter_on_file 004_word_frequency_counter_on_file.cpp
//   ./004_word_frequency_counter_on_file 004_file.txt
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

// Normalize a token to lowercase and strip leading/trailing non-alnum.
std::string clean(std::string s) {
    // to lowercase
    for (auto& ch : s)
        ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));

    // trim non-alnum at both ends
    auto isok = [](unsigned char c) { return std::isalnum(c) != 0; };
    auto it1 = std::find_if(s.begin(), s.end(), isok);
    auto it2 = std::find_if(s.rbegin(), s.rend(), isok).base();
    if (it1 >= it2) return {};
    return std::string(it1, it2);
}

int main(int argc, char** argv) {
    std::istream* in = &std::cin;
    std::ifstream f;
    if (argc > 1) {
        f.open(argv[1]);
        if (!f) {
            std::cerr << "Failed to open: " << argv[1] << "\n";
            return 1;
        }
        in = &f;
    }

    // 1) Count with std::map (ordered by key, deterministic traversal)
    std::map<std::string, std::size_t> freq;

    std::string tok;
    while (*in >> tok) {
        auto w = clean(tok);
        if (!w.empty()) ++freq[w];
    }

    // 2) Copy to vector for sorting by frequency (desc), then by word (asc)
    std::vector<std::pair<std::string, std::size_t>> items(freq.begin(), freq.end());

    std::sort(items.begin(), items.end(),
              [](const auto& a, const auto& b) {
                  if (a.second != b.second) return a.second > b.second; // higher count first
                  return a.first < b.first;                              // tie-break by word
              });

    // 3) Print (top 20 by default)
    std::size_t limit = 20;
    for (std::size_t i = 0; i < std::min(limit, items.size()); ++i) {
        std::cout << items[i].first << " : " << items[i].second << "\n";
    }
}

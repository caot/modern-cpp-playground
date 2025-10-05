/*
    This file provides a complete example demonstrating:

    * Concurrency (std::async): Launches three separate analysis functions simultaneously.
    * Futures (std::future): Manages the asynchronous operation and waits to retrieve the final results (.get()).
    * Data Processing: Simulates calculating complex metrics (average and volatility/standard deviation) on data chunks.
    * Reporting: Combines and sorts the results to create a final, meaningful report (sorted by volatility).
 */
#include <iostream>
#include <vector>
#include <string>
#include <future>
#include <thread>
#include <chrono>
#include <numeric>
#include <cmath>
#include <iomanip> // ADDED: Required for std::fixed and std::setprecision

using namespace std::chrono_literals;

/**
 * @brief Represents the result of analyzing a single data chunk.
 */
struct AnalysisResult {
    std::string stock_symbol;
    double average_price;
    double volatility;
    size_t data_points;
};

/**
 * @brief Simulates intensive analysis on a chunk of stock data.
 * * This function performs the concurrent work for each thread/task.
 * It simulates calculation of average price and standard deviation (volatility).
 * * @param symbol The stock symbol being analyzed.
 * @param chunk A vector of daily closing prices.
 * @return AnalysisResult The structured result of the analysis.
 */
AnalysisResult analyze_chunk(const std::string& symbol, const std::vector<double>& chunk) {
    // Simulate work delay to make concurrency observable
    std::this_thread::sleep_for(100ms);

    std::cout << "  [THREAD " << std::this_thread::get_id() << "] Starting analysis for " << symbol
              << " (" << chunk.size() << " data points)." << std::endl;

    if (chunk.empty()) {
        return {symbol, 0.0, 0.0, 0};
    }

    // 1. Calculate Average Price (Mean)
    double sum = std::accumulate(chunk.begin(), chunk.end(), 0.0);
    double avg = sum / chunk.size();

    // 2. Calculate Volatility (Standard Deviation)
    double squared_diff_sum = 0.0;
    for (double price : chunk) {
        squared_diff_sum += std::pow(price - avg, 2);
    }
    double variance = squared_diff_sum / chunk.size();
    double volatility = std::sqrt(variance);

    std::this_thread::sleep_for(50ms); // Simulate result compilation delay

    std::cout << "  [THREAD " << std::this_thread::get_id() << "] Finished analysis for " << symbol << std::endl;

    return {symbol, avg, volatility, chunk.size()};
}

int main() {
    std::cout << "=== Concurrent Stock Data Analysis Demo ===" << std::endl;
    std::cout << "System Concurrency: " << std::thread::hardware_concurrency() << " cores." << std::endl;

    // Simulated Stock Data Chunks (Fetching from web would happen here)
    std::vector<double> tsla_data = {250.1, 255.5, 248.9, 260.0, 265.1};
    std::vector<double> goog_data = {140.2, 140.8, 139.5, 142.1, 141.5};
    std::vector<double> amzn_data = {175.3, 175.0, 176.5, 174.9, 177.0};

    // --- 1. Launch Concurrent Tasks using std::async ---
    // std::async launches the analyze_chunk function potentially on a separate thread (std::launch::async).
    std::cout << "\n--- Launching 3 Concurrent Analysis Tasks ---" << std::endl;

    // We store the results in std::future objects.
    std::future<AnalysisResult> future_tsla = std::async(std::launch::async, analyze_chunk, "TSLA", std::ref(tsla_data));
    std::future<AnalysisResult> future_goog = std::async(std::launch::async, analyze_chunk, "GOOG", std::ref(goog_data));
    std::future<AnalysisResult> future_amzn = std::async(std::launch::async, analyze_chunk, "AMZN", std::ref(amzn_data));

    // --- 2. Wait for and Retrieve Results using std::future ---
    std::cout << "\n--- Waiting for all tasks to complete... ---" << std::endl;

    // .get() blocks until the result is available. Because we launched them async,
    // this wait time will be concurrent.
    AnalysisResult res_tsla = future_tsla.get();
    AnalysisResult res_goog = future_goog.get();
    AnalysisResult res_amzn = future_amzn.get();

    std::cout << "All tasks retrieved successfully." << std::endl;

    // --- 3. Generate Meaningful Report ---
    std::cout << "\n=== Final Consolidated Market Volatility Report ===" << std::endl;
    std::cout << "---------------------------------------------------" << std::endl;

    std::vector<AnalysisResult> all_results = {res_tsla, res_goog, res_amzn};

    // Sort results by volatility (highest first)
    std::sort(all_results.begin(), all_results.end(),
        [](const AnalysisResult& a, const AnalysisResult& b) {
            return a.volatility > b.volatility;
        });

    for (const auto& res : all_results) {
        std::cout << res.stock_symbol << ":\n";
        std::cout << "  - Avg. Price: " << std::fixed << std::setprecision(2) << res.average_price << "\n";
        std::cout << "  - Volatility (St. Dev): " << std::fixed << std::setprecision(4) << res.volatility << " (Index of risk)\n";
        std::cout << "  - Data Points: " << res.data_points << "\n";
    }
    std::cout << "---------------------------------------------------" << std::endl;
    std::cout << "Report complete." << std::endl;

    return 0;
}

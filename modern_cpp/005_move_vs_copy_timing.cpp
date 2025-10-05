// move_vs_copy_timing.cpp
// g++ -std=c++20 -O2 -march=native -o ../output/005_move_vs_copy_timing 005_move_vs_copy_timing.cpp
// ../output/005_move_vs_copy_timing
#include <atomic>
#include <chrono>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

struct BigData {
    // Large heap buffer
    std::size_t n{};
    std::unique_ptr<int[]> buf;

    inline static std::size_t copy_ctor_count = 0;
    inline static std::size_t copy_assign_count = 0;
    inline static std::size_t move_ctor_count = 0;
    inline static std::size_t move_assign_count = 0;

    explicit BigData(std::size_t n_)
        : n(n_), buf(n_ ? std::make_unique<int[]>(n_) : nullptr) {
        for (std::size_t i = 0; i < n; ++i) buf[i] = static_cast<int>(i);
    }

    // Deep copy (expensive)
    BigData(const BigData& other) : n(other.n), buf(other.n ? std::make_unique<int[]>(other.n) : nullptr) {
        ++copy_ctor_count;
        if (n) std::memcpy(buf.get(), other.buf.get(), n * sizeof(int));
    }

    BigData& operator=(const BigData& other) {
        ++copy_assign_count;
        if (this == &other) return *this;
        if (other.n) {
            auto tmp = std::make_unique<int[]>(other.n);
            std::memcpy(tmp.get(), other.buf.get(), other.n * sizeof(int));
            buf = std::move(tmp);
        } else {
            buf.reset();
        }
        n = other.n;
        return *this;
    }

    // Move (cheap)
    BigData(BigData&& other) noexcept : n(other.n), buf(std::move(other.buf)) {
        ++move_ctor_count;
        other.n = 0;
    }

    BigData& operator=(BigData&& other) noexcept {
        ++move_assign_count;
        if (this == &other) return *this;
        buf = std::move(other.buf);
        n   = other.n;
        other.n = 0;
        return *this;
    }

    // Some work so the optimizer can't drop everything
    long long sum() const {
        long long s = 0;
        for (std::size_t i = 0; i < n; ++i) s += buf[i];
        return s;
    }
};

// Prevent “work” from being optimized away
static std::atomic<long long> sink{0};

// ---------- Generic function template (pass-by-value) ----------
// Taking by value forces either a copy (from lvalue) or a move (from rvalue).
template <class T>
void consume_by_value(T t) {
    sink.fetch_add(t.sum(), std::memory_order_relaxed);
}

// ---------- Tiny timing helper ----------
template <class F>
auto time_run(F&& f, std::size_t iters) {
    const auto start = std::chrono::steady_clock::now();
    for (std::size_t i = 0; i < iters; ++i) f();
    const auto stop  = std::chrono::steady_clock::now();
    return std::chrono::duration<double, std::milli>(stop - start).count();
}

int main() {
    constexpr std::size_t N     = 2'000'000;  // ~8MB buffer (2M * 4 bytes)
    constexpr std::size_t ITERS = 50;

    std::cout << "Buffer size: " << N << " ints (" << (N * sizeof(int)) / (1024 * 1024) << " MiB)\n";
    std::cout << "Iterations : " << ITERS << "\n\n";

    // ---------- Copy scenario: passing an lvalue repeatedly ----------
    BigData big(N); // construct once; loop measures ONLY passing-by-value (copy)
    BigData::copy_ctor_count = BigData::copy_assign_count = 0;
    BigData::move_ctor_count = BigData::move_assign_count = 0;

    auto ms_copy = time_run([&] {
        consume_by_value(big); // forces COPY construction of parameter T t
    }, ITERS);

    std::cout << "[COPY] total: " << std::fixed << std::setprecision(2)
              << ms_copy << " ms  | avg: " << (ms_copy / ITERS) << " ms/iter\n";
    std::cout << "       copy-ctor: " << BigData::copy_ctor_count
              << ", copy-assign: "    << BigData::copy_assign_count
              << ", move-ctor: "      << BigData::move_ctor_count
              << ", move-assign: "    << BigData::move_assign_count << "\n\n";

    // ---------- Move scenario: passing rvalues (prepared in a pool) ----------
    // Prepare a pool of BigData objects; we will MOVE each element once.
    std::vector<BigData> pool;
    pool.reserve(ITERS);
    for (std::size_t i = 0; i < ITERS; ++i) pool.emplace_back(N);

    BigData::copy_ctor_count = BigData::copy_assign_count = 0;
    BigData::move_ctor_count = BigData::move_assign_count = 0;

    std::size_t idx = 0;
    auto ms_move = time_run([&] {
        // Move the next object into the by-value parameter (cheap)
        consume_by_value(std::move(pool[idx]));
        ++idx;
    }, ITERS);

    std::cout << "[MOVE] total: " << std::fixed << std::setprecision(2)
              << ms_move << " ms  | avg: " << (ms_move / ITERS) << " ms/iter\n";
    std::cout << "       copy-ctor: " << BigData::copy_ctor_count
              << ", copy-assign: "    << BigData::copy_assign_count
              << ", move-ctor: "      << BigData::move_ctor_count
              << ", move-assign: "    << BigData::move_assign_count << "\n\n";

    // Show that some work happened so the compiler can't optimize it all away
    std::cout << "(ignore) sink = " << sink << "\n";

    std::cout << "\nExpected: COPY significantly slower than MOVE; "
                 "move-ctor count >> 0 and copy-ctor count == ITERS in copy test.\n";
}

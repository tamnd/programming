```cpp run id=master-worker-pattern
#include <algorithm>
#include <future>
#include <iostream>
#include <utility>
#include <vector>

constexpr int kLimit = 20000;
constexpr int kWorkers = 4;

// The work of one worker: count the primes in [lo, hi).
int count_primes(int lo, int hi) {
    int count = 0;
    for (int n = std::max(lo, 2); n < hi; n++) {
        int d = 2;
        while (d * d <= n && n % d != 0) d++;
        if (d * d > n) count++;
    }
    return count;
}

int main() {
    // 1. Split the work into one part for each worker.
    std::vector<std::pair<int, int>> parts;
    for (int i = 0; i < kWorkers; i++) parts.push_back({kLimit * i / kWorkers, kLimit * (i + 1) / kWorkers});
    // 2. Start a thread for each part.
    std::vector<std::future<int>> futures;
    for (auto [lo, hi] : parts) futures.push_back(std::async(std::launch::async, count_primes, lo, hi));
    // 3. Wait for each result and combine the partial results.
    std::vector<int> results;
    int total = 0;
    for (auto& f : futures) {
        results.push_back(f.get());
        total += results.back();
    }

    std::cout << "parts: [";
    for (std::size_t i = 0; i < parts.size(); i++)
        std::cout << (i ? ", " : "") << "(" << parts[i].first << ", " << parts[i].second << ")";
    std::cout << "]\nprimes in each part: [";
    for (std::size_t i = 0; i < results.size(); i++) std::cout << (i ? ", " : "") << results[i];
    std::cout << "]\nprimes below " << kLimit << " from the master: " << total << "\n";
    std::cout << "same as one loop: " << (total == count_primes(0, kLimit) ? "True" : "False") << "\n";
}
```

```output
parts: [(0, 5000), (5000, 10000), (10000, 15000), (15000, 20000)]
primes in each part: [669, 560, 525, 508]
primes below 20000 from the master: 2262
same as one loop: True
```

- Verdict: stdlib. `std::async` with `std::launch::async` starts a thread for each part, and the master calls `get` on each `std::future`.

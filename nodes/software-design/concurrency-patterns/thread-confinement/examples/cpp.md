```cpp run id=thread-confinement
#include <iostream>
#include <map>
#include <string>
#include <thread>
#include <vector>

int main() {
    const std::vector<std::string> words = {"a", "b", "c", "d", "e"};
    const int chunks = 4;
    std::vector<std::map<std::string, int>> results(chunks);
    std::vector<std::thread> threads;
    for (int i = 0; i < chunks; i++) {
        threads.emplace_back([&words, &results, i] {
            std::map<std::string, int> counts; // local: no lock
            for (int r = 0; r < 2000; r++) {
                for (const auto& w : words) counts[w]++;
            }
            results[i] = std::move(counts); // hand the finished object over
        });
    }
    for (auto& t : threads) t.join(); // after join, main owns the results

    std::map<std::string, int> total;
    for (const auto& counts : results) {
        for (const auto& [w, n] : counts) total[w] += n;
    }
    int sum = 0;
    for (const auto& [w, n] : total) sum += n;
    std::cout << "chunks: " << chunks << "\n";
    std::cout << "words counted: " << sum << "\n";
    std::cout << "distinct words: " << total.size() << "\n";
    std::cout << "count of 'a': " << total["a"] << "\n";
}
```

```output
chunks: 4
words counted: 40000
distinct words: 5
count of 'a': 8000
```

- Verdict: idiom. Each thread fills a local `std::map` and moves it into its own slot of `results`, and `join` makes the main thread the only owner.

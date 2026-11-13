```cpp run id=barrier-pattern
#include <barrier>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

constexpr int N = 8;
int values[N];
const std::vector<int> distances = {1, 2, 4};  // each step adds the value that is d places to the left

std::string show() {
    std::string s = "[";
    for (int i = 0; i < N; i++) s += (i ? ", " : "") + std::to_string(values[i]);
    return s + "]";
}

int main() {
    for (int i = 0; i < N; i++) values[i] = i + 1;
    std::cout << "input: " << show() << "\n";
    int trips = 0;
    // The completion function runs once per phase, before the threads are released.
    auto count_trip = [&]() noexcept { trips++; };
    std::barrier barrier(N, count_trip);

    std::vector<std::thread> threads;
    for (int i = 0; i < N; i++) {
        threads.emplace_back([&, i] {
            for (int d : distances) {
                // Phase 1: read a value that the last step wrote.
                int add = i >= d ? values[i - d] : 0;
                barrier.arrive_and_wait();  // nobody writes until every worker has read
                // Phase 2: write the new value.
                values[i] += add;
                barrier.arrive_and_wait();  // nobody reads until every worker has written
            }
        });
    }
    for (auto& t : threads) t.join();

    bool matches = true;
    for (int i = 0; i < N; i++) matches = matches && values[i] == (i + 1) * (i + 2) / 2;
    std::cout << "steps: " << distances.size() << "\n";
    std::cout << "barrier trips: " << trips << "\n";
    std::cout << "prefix sums: " << show() << "\n";
    std::cout << "matches a running total: " << (matches ? "True" : "False") << "\n";
}
```

```output
input: [1, 2, 3, 4, 5, 6, 7, 8]
steps: 3
barrier trips: 6
prefix sums: [1, 3, 6, 10, 15, 21, 28, 36]
matches a running total: True
```

- Verdict: stdlib. `std::barrier` with a completion function and `arrive_and_wait`.

```cpp run id=thread-pool
#include <atomic>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <thread>
#include <vector>

int main() {
    constexpr int kTasks = 1000;
    constexpr int kWorkers = 10;
    std::atomic<int> next{0};
    std::atomic<std::int64_t> sum{0};
    {
        // Ten OS threads take tasks from a shared counter until none are left.
        std::vector<std::jthread> workers;
        for (int w = 0; w < kWorkers; ++w) {
            workers.emplace_back([&] {
                for (int n = next.fetch_add(1); n < kTasks; n = next.fetch_add(1)) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(10));  // blocks this OS thread
                    sum += n;
                }
            });
        }
    }  // each jthread joins here
    std::cout << "tasks: " << kTasks << ", sum: " << sum << '\n';
    std::cout << "OS threads: " << kWorkers << ", so never more than " << kWorkers << " at once\n";
}
```

```output for=thread-pool
tasks: 1000, sum: 499500
OS threads: 10, so never more than 10 at once
```

- Verdict: absent. A `std::thread` or `std::jthread` is an OS thread, and the standard library has no M:N scheduler. A fixed set of OS threads is the usual alternative.
- Ten threads take task numbers from an atomic counter. The limit of ten comes from the number of threads.
- `sleep_for` blocks the whole OS thread, so the threads cannot run other tasks while they wait.
- The vector of `std::jthread` objects goes out of scope at the end of the inner block, and each destructor joins its thread before `main` reads the sum.

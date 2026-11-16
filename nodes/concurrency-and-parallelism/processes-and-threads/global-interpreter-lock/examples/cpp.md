```cpp run id=threads-and-the-gil
#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

int main() {
    // C++ threads have no global lock, so they can run at the same time.
    // A plain shared long changed by all threads would be a data race. std::atomic is not.
    std::atomic<long> total{0};
    {
        std::vector<std::jthread> threads;
        for (int t = 0; t < 4; t++) {
            threads.emplace_back([&total] {
                for (int i = 0; i < 100'000; i++) {
                    total.fetch_add(1, std::memory_order_relaxed);
                }
            });
        }
    }  // the jthread destructors join the threads here
    std::cout << "4 threads added 100000 each with std::atomic\n";
    std::cout << "total: " << total.load() << "\n";
}
```

```output for=threads-and-the-gil
4 threads added 100000 each with std::atomic
total: 400000
```

- Verdict: absent. C++ has no GIL. Threads run at the same time, and a plain shared `long` that they all change is a data race.
- With `std::atomic`, "If one thread writes to an atomic object while another thread reads from it, the behavior is well-defined".
- The `std::jthread` objects are in a block, and their destructors join the threads before the total is printed.

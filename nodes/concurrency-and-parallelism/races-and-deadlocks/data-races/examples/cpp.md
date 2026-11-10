```cpp run id=atomic
#include <atomic>
#include <print>
#include <thread>
#include <vector>

int main() {
    std::atomic<int> count = 0;
    {
        std::vector<std::jthread> threads;
        for (int t = 0; t < 2; t++) {
            threads.emplace_back([&count] {
                for (int i = 0; i < 100'000; i++) {
                    count++;
                }
            });
        }
    }
    std::println("{}", count.load());
}
```

```output for=atomic
200000
```

```cpp fails expect=sanitizer:thread flags="-fsanitize=thread" id=race
#include <thread>

int main() {
    int count = 0;
    auto work = [&count] {
        for (int i = 0; i < 100'000; i++) {
            count++;
        }
    };
    std::thread a(work);
    std::thread b(work);
    a.join();
    b.join();
}
```

- Verdict: stdlib. `std::atomic<int>` makes `count++` one atomic read-modify-write with sequentially consistent order.
- A `std::jthread` joins in its destructor, so the threads finish when the inner block ends, before `count.load()`.
- The second program increments a plain `int` from two threads. Any data race in C++ is undefined behaviour, so it has no output block. ThreadSanitizer reports the race.

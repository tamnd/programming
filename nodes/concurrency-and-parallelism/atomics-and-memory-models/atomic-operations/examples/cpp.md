```cpp run id=atomic
#include <atomic>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

std::atomic<int> hits{0};
std::string message;
std::atomic<bool> ready{false};

int main() {
    {
        std::vector<std::jthread> workers;
        for (int t = 0; t < 4; ++t) {
            workers.emplace_back([] {
                for (int i = 0; i < 100000; ++i) {
                    hits.fetch_add(1, std::memory_order_relaxed);
                }
            });
        }
    }
    std::cout << hits.load() << "\n";

    int expected = 400000;
    bool swapped = hits.compare_exchange_strong(expected, 0);
    std::cout << std::boolalpha << swapped << " " << hits.load() << "\n";

    std::jthread producer([] {
        message = "hello";
        ready.store(true, std::memory_order_release);
    });
    while (!ready.load(std::memory_order_acquire)) {
    }
    std::cout << message << "\n";
}
```

```output for=atomic
400000
true 0
hello
```

- Verdict: stdlib. `std::atomic<T>` makes loads, stores and read-modify-write operations atomic, and each operation can take a `std::memory_order`. The default is `memory_order_seq_cst`.
- The release store and the acquire load make the plain write to `message` visible to the main thread. Without them, the access to `message` would be a data race.
- `std::atomic_ref` gives atomic access to an object that is not declared atomic, for example an element of a large array.

```cpp norun reason=ub id=plain-counter
#include <iostream>
#include <thread>

int hits = 0;

int main() {
    std::thread a([] { for (int i = 0; i < 100000; ++i) ++hits; });
    std::thread b([] { for (int i = 0; i < 100000; ++i) ++hits; });
    a.join();
    b.join();
    std::cout << hits << "\n";
}
```

Two threads increment a plain `int` without synchronization. This is a data race and undefined behavior. In practice, the program often prints a number below 200000, because increments are lost, and an optimizer can also merge the loop into one addition. ThreadSanitizer, `-fsanitize=thread`, reports the race.

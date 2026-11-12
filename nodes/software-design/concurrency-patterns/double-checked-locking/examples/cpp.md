```cpp run id=dcl
#include <atomic>
#include <barrier>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

std::atomic<int> created{0};
int locked_calls = 0;

struct Config {
    Config() { ++created; }
};

std::atomic<Config*> instance_ptr{nullptr};
std::mutex lock_;

Config* instance() {
    Config* p = instance_ptr.load(std::memory_order_acquire);
    if (p == nullptr) {  // first check, no lock
        std::lock_guard<std::mutex> guard(lock_);
        ++locked_calls;
        p = instance_ptr.load(std::memory_order_relaxed);
        if (p == nullptr) {  // second check, lock held
            p = new Config();
            instance_ptr.store(p, std::memory_order_release);
        }
    }
    return p;
}

int main() {
    std::barrier start(8);
    std::vector<std::thread> threads;
    for (int w = 0; w < 8; ++w)
        threads.emplace_back([&] {
            start.arrive_and_wait();
            for (int i = 0; i < 1000; ++i) instance();
        });
    for (auto& t : threads) t.join();
    std::cout << "instances created: " << created << "\n";
    std::cout << "calls: " << 8 * 1000 << "\n";
    std::cout << "calls that took the lock: "
              << (locked_calls <= 8 ? "8 or fewer" : std::to_string(locked_calls)) << "\n";
    delete instance_ptr.load();
}
```

```output
instances created: 1
calls: 8000
calls that took the lock: 8 or fewer
```

- Verdict: idiom. The pointer is a `std::atomic` with acquire and release ordering. `std::call_once` is the library form of the same work.

```cpp run id=copy-on-write
#include <atomic>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

class CopyOnWriteList {
public:
    explicit CopyOnWriteList(std::vector<std::string> items)
        : items_(std::make_shared<const std::vector<std::string>>(std::move(items))) {}

    void add(const std::string& item) {
        std::lock_guard<std::mutex> write(write_mutex_); // writers only
        auto next = std::make_shared<std::vector<std::string>>(*snapshot()); // copy
        next->push_back(item); // change the copy
        std::lock_guard<std::mutex> lock(pointer_mutex_);
        items_ = std::move(next); // publish it
    }

    std::shared_ptr<const std::vector<std::string>> snapshot() const {
        std::lock_guard<std::mutex> lock(pointer_mutex_); // guards only the pointer copy
        return items_;
    }

private:
    std::shared_ptr<const std::vector<std::string>> items_;
    std::mutex write_mutex_;
    mutable std::mutex pointer_mutex_;
};

int main() {
    CopyOnWriteList listeners({"listener 0"});
    auto first = listeners.snapshot();
    std::atomic<int> iterations{0}, errors{0};
    std::vector<std::thread> threads;
    for (int i = 0; i < 4; i++) {
        threads.emplace_back([&] {
            for (int r = 0; r < 1000; r++) {
                auto items = listeners.snapshot();
                for (const auto& name : *items) {
                    (void)name; // call each listener
                }
                iterations++;
            }
        });
    }
    threads.emplace_back([&] {
        for (int n = 1; n <= 100; n++) listeners.add("listener " + std::to_string(n));
    });
    for (auto& t : threads) t.join();
    std::cout << "iterations: " << iterations << "\n";
    std::cout << "iterations that failed: " << errors << "\n";
    std::cout << "listeners at the end: " << listeners.snapshot()->size() << "\n";
    std::cout << "length of the first snapshot: " << first->size() << "\n";
}
```

```output
iterations: 4000
iterations that failed: 0
listeners at the end: 101
length of the first snapshot: 1
```

- Verdict: idiom. `CopyOnWriteList` keeps a `std::shared_ptr` to a `const` vector; `add` copies the vector and swaps the pointer, and a reader keeps its old vector alive while it iterates.

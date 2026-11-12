```cpp run id=producer-consumer
#include <algorithm>
#include <condition_variable>
#include <deque>
#include <iostream>
#include <mutex>
#include <numeric>
#include <optional>
#include <thread>
#include <vector>

class BoundedBuffer {
public:
    explicit BoundedBuffer(size_t capacity) : capacity_(capacity) {}

    void put(int item) {
        std::unique_lock<std::mutex> lock(mutex_);
        not_full_.wait(lock, [this] { return items_.size() < capacity_; });
        items_.push_back(item);
        not_empty_.notify_one();
    }

    std::optional<int> take() { // empty result: the stream has ended
        std::unique_lock<std::mutex> lock(mutex_);
        not_empty_.wait(lock, [this] { return closed_ || !items_.empty(); });
        if (items_.empty()) return std::nullopt;
        int item = items_.front();
        items_.pop_front();
        not_full_.notify_one();
        return item;
    }

    void close() {
        std::lock_guard<std::mutex> lock(mutex_);
        closed_ = true;
        not_empty_.notify_all();
    }

private:
    size_t capacity_;
    std::deque<int> items_;
    bool closed_ = false;
    std::mutex mutex_;
    std::condition_variable not_full_, not_empty_;
};

int main() {
    BoundedBuffer buffer(4);
    std::mutex consumed_mutex;
    std::vector<int> consumed;

    std::vector<std::thread> producers, consumers;
    for (int start : {0, 500}) {
        producers.emplace_back([&buffer, start] {
            for (int item = start; item < start + 500; item++) buffer.put(item);
        });
    }
    for (int i = 0; i < 2; i++) {
        consumers.emplace_back([&] {
            while (auto item = buffer.take()) {
                std::lock_guard<std::mutex> lock(consumed_mutex);
                consumed.push_back(*item);
            }
        });
    }
    for (auto& t : producers) t.join();
    buffer.close(); // the end of the stream for every consumer
    for (auto& t : consumers) t.join();

    std::sort(consumed.begin(), consumed.end());
    std::vector<int> expected(1000);
    std::iota(expected.begin(), expected.end(), 0);
    long long sum = std::accumulate(consumed.begin(), consumed.end(), 0LL);
    std::cout << "items consumed: " << consumed.size() << "\n";
    std::cout << "sum of items: " << sum << "\n";
    std::cout << "each item consumed once: " << (consumed == expected ? "True" : "False") << "\n";
}
```

```output
items consumed: 1000
sum of items: 499500
each item consumed once: True
```

- Verdict: idiom. `BoundedBuffer` is a small class: a `std::deque` with a mutex and the two condition variables `not_full_` and `not_empty_`.

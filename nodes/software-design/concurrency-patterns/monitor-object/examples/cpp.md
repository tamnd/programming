```cpp run id=monitor
#include <algorithm>
#include <condition_variable>
#include <deque>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

class MessageQueue {
public:
    explicit MessageQueue(std::size_t capacity) : capacity_(capacity) {}

    void put(int message) {
        std::unique_lock<std::mutex> lock(mutex_);
        // wait releases the lock, and the predicate is checked again on wakeup.
        not_full_.wait(lock, [this] { return items_.size() < capacity_; });
        items_.push_back(message);
        ++puts;
        largest = std::max(largest, items_.size());
        not_empty_.notify_one();
    }

    int get() {
        std::unique_lock<std::mutex> lock(mutex_);
        not_empty_.wait(lock, [this] { return !items_.empty(); });
        int message = items_.front();
        items_.pop_front();
        ++gets;
        not_full_.notify_one();
        return message;
    }

    int puts = 0;
    int gets = 0;
    std::size_t largest = 0;

private:
    std::size_t capacity_;
    std::deque<int> items_;
    std::mutex mutex_;
    std::condition_variable not_full_;
    std::condition_variable not_empty_;
};

int main() {
    MessageQueue q(2);
    long sums[2] = {0, 0};
    std::vector<std::thread> threads;
    for (int p = 0; p < 4; ++p)
        threads.emplace_back([&q, p] { for (int i = 0; i < 250; ++i) q.put(p * 250 + i); });
    for (int c = 0; c < 2; ++c)
        threads.emplace_back([&q, &sums, c] { for (int i = 0; i < 500; ++i) sums[c] += q.get(); });
    for (auto& t : threads) t.join();
    std::cout << "messages put: " << q.puts << "\n";
    std::cout << "messages got: " << q.gets << "\n";
    std::cout << "sum of messages got: " << sums[0] + sums[1] << "\n";
    std::cout << "queue never held more than 2: " << (q.largest <= 2 ? "True" : "False") << "\n";
}
```

```output
messages put: 1000
messages got: 1000
sum of messages got: 499500
queue never held more than 2: True
```

- Verdict: stdlib. `std::mutex` is the monitor lock and two `std::condition_variable` objects are the monitor conditions. The class keeps the lock private, so clients cannot skip it.

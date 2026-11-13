```cpp run id=pipeline-concurrency
#include <atomic>
#include <condition_variable>
#include <deque>
#include <iostream>
#include <mutex>
#include <optional>
#include <thread>
#include <vector>

// A bounded channel: send waits while the buffer is full, and close ends the stream.
template <typename T>
class Channel {
public:
    explicit Channel(size_t capacity) : capacity_(capacity) {}
    void send(T v) {
        std::unique_lock<std::mutex> lock(m_);
        cv_.wait(lock, [&] { return buf_.size() < capacity_; });
        buf_.push_back(v);
        cv_.notify_all();
    }
    std::optional<T> receive() {  // empty once the channel is closed and drained
        std::unique_lock<std::mutex> lock(m_);
        cv_.wait(lock, [&] { return !buf_.empty() || closed_; });
        if (buf_.empty()) return std::nullopt;
        T v = buf_.front();
        buf_.pop_front();
        cv_.notify_all();
        return v;
    }
    void close() {
        std::lock_guard<std::mutex> lock(m_);
        closed_ = true;
        cv_.notify_all();
    }

private:
    std::mutex m_;
    std::condition_variable cv_;
    std::deque<T> buf_;
    size_t capacity_;
    bool closed_ = false;
};

constexpr int kItems = 20;
constexpr int kWorkers = 3;

int main() {
    Channel<int> numbers(1);  // source -> square workers
    Channel<int> squares(1);  // square workers -> sink
    std::atomic<bool> sink_started{false};
    bool overlap = false;

    std::thread source([&] {
        for (int n = 1; n <= kItems; n++) numbers.send(n);
        // Each stage can hold only a few values, so the sink must already be running.
        overlap = sink_started.load();
        numbers.close();
    });
    std::atomic<int> running{kWorkers};
    std::vector<std::thread> workers;  // fan-out
    for (int w = 0; w < kWorkers; w++) {
        workers.emplace_back([&] {
            while (auto n = numbers.receive()) squares.send(*n * *n);
            if (--running == 0) squares.close();  // the last worker closes the output: fan-in
        });
    }

    int received = 0, total = 0;
    while (auto v = squares.receive()) {
        sink_started = true;
        received++;
        total += *v;
    }
    source.join();
    for (auto& t : workers) t.join();

    std::cout << "items from the source: " << kItems << "\n";
    std::cout << "square workers: " << kWorkers << "\n";
    std::cout << "squares that reached the sink: " << received << "\n";
    std::cout << "sum of squares: " << total << "\n";
    std::cout << "the sink got a value before the source sent its last: " << (overlap ? "True" : "False") << "\n";
}
```

```output
items from the source: 20
square workers: 3
squares that reached the sink: 20
sum of squares: 2870
the sink got a value before the source sent its last: True
```

- Verdict: idiom. The standard library has no channel, so the example has a small bounded channel class with `close`, built on `std::mutex` and `std::condition_variable`.

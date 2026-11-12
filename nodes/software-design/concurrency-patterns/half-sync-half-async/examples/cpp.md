```cpp run id=hsha
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <deque>
#include <iostream>
#include <mutex>
#include <optional>
#include <thread>
#include <vector>

// The queueing layer: put never blocks, get blocks until an item or close.
class RequestQueue {
public:
    void put(int request) {
        { std::lock_guard<std::mutex> lock(mutex_); items_.push_back(request); }
        ready_.notify_one();
    }
    std::optional<int> get() {
        std::unique_lock<std::mutex> lock(mutex_);
        ready_.wait(lock, [this] { return !items_.empty() || closed_; });
        if (items_.empty()) return std::nullopt;
        int request = items_.front();
        items_.pop_front();
        return request;
    }
    void close() {
        { std::lock_guard<std::mutex> lock(mutex_); closed_ = true; }
        ready_.notify_all();
    }
    std::size_t size() {
        std::lock_guard<std::mutex> lock(mutex_);
        return items_.size();
    }

private:
    std::mutex mutex_;
    std::condition_variable ready_;
    std::deque<int> items_;
    bool closed_ = false;
};

int main() {
    RequestQueue requests;
    std::mutex gate_mutex;
    std::condition_variable gate_cv;
    bool gate_open = false; // lets the demo hold the sync layer
    std::atomic<int> done{0};
    std::atomic<long> sum{0};
    int received = 0;

    std::vector<std::thread> workers;
    for (int i = 0; i < 4; ++i) {
        workers.emplace_back([&] {
            {
                std::unique_lock<std::mutex> lock(gate_mutex);
                gate_cv.wait(lock, [&] { return gate_open; });
            }
            while (auto request = requests.get()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1)); // a blocking call
                ++done;
                sum += *request;
            }
        });
    }
    // Async layer: a short handler that never blocks.
    auto on_event = [&](int event_id) { ++received; requests.put(event_id); };
    std::thread event_source([&] { for (int id = 0; id < 100; ++id) on_event(id); });
    event_source.join();
    std::cout << "events received by the async layer: " << received << "\n";
    std::cout << "requests in the queueing layer before the sync layer starts: " << requests.size() << "\n";
    { std::lock_guard<std::mutex> lock(gate_mutex); gate_open = true; }
    gate_cv.notify_all();
    requests.close();
    for (auto& w : workers) w.join();
    std::cout << "requests done by the sync layer: " << done << "\n";
    std::cout << "sum of request ids: " << sum << "\n";
}
```

```output
events received by the async layer: 100
requests in the queueing layer before the sync layer starts: 100
requests done by the sync layer: 100
sum of request ids: 4950
```

- Verdict: library. The standard library has no blocking queue, so `RequestQueue` builds the queueing layer from `std::mutex` and `std::condition_variable`.

```cpp run id=queue
#include <condition_variable>
#include <deque>
#include <mutex>
#include <optional>
#include <print>
#include <thread>
#include <vector>

template <typename T>
class Channel {
public:
    explicit Channel(std::size_t cap) : cap_(cap) {}
    void send(T v) {
        std::unique_lock lock(m_);
        not_full_.wait(lock, [&] { return q_.size() < cap_; });
        q_.push_back(std::move(v));
        not_empty_.notify_one();
    }
    std::optional<T> receive() { // empty optional: closed and drained
        std::unique_lock lock(m_);
        not_empty_.wait(lock, [&] { return !q_.empty() || closed_; });
        if (q_.empty()) return std::nullopt;
        T v = std::move(q_.front());
        q_.pop_front();
        not_full_.notify_one();
        return v;
    }
    void close() {
        std::lock_guard lock(m_);
        closed_ = true;
        not_empty_.notify_all();
    }

private:
    std::mutex m_;
    std::condition_variable not_full_, not_empty_;
    std::deque<T> q_;
    std::size_t cap_;
    bool closed_ = false;
};

int main() {
    Channel<int> jobs(2);
    std::vector<std::jthread> producers;
    for (int id = 1; id <= 2; ++id) {
        producers.emplace_back([&jobs, id] {
            for (int i = 1; i <= 3; ++i) jobs.send(id * 10 * i);
        });
    }
    std::jthread closer([&] {
        for (auto& p : producers) p.join();
        jobs.close();
    });
    int total = 0;
    while (auto v = jobs.receive()) total += *v;
    std::println("total: {}", total);
}
```

```output for=queue
total: 180
```

- Verdict: idiom. The C++ standard library has no channel. A queue guarded by a `std::mutex`, with one `std::condition_variable` for "not full" and one for "not empty", is the usual form.
- `wait` with a predicate loops until the condition holds, because a wait can also return spuriously.
- This sketch has no rendezvous and no `select`; each of those needs more state.

```cpp run id=channel-from-mutex-and-condvar
#include <condition_variable>
#include <deque>
#include <mutex>
#include <optional>
#include <print>
#include <string>
#include <thread>
#include <vector>

// The standard library has no channel, so this one is built from a mutex,
// a condition variable and a queue.
template <typename T>
class Channel {
public:
    void send(T value) {
        {
            std::lock_guard lock(m_);
            q_.push_back(std::move(value));
        }
        cv_.notify_one();
    }

    void close() {
        {
            std::lock_guard lock(m_);
            closed_ = true;
        }
        cv_.notify_all();
    }

    // Waits for a message, and gives nullopt when the channel is closed and empty.
    std::optional<T> receive() {
        std::unique_lock lock(m_);
        cv_.wait(lock, [this] { return !q_.empty() || closed_; });
        if (q_.empty()) {
            return std::nullopt;
        }
        T v = std::move(q_.front());
        q_.pop_front();
        return v;
    }

private:
    std::mutex m_;
    std::condition_variable cv_;
    std::deque<T> q_;
    bool closed_ = false;
};

int main() {
    Channel<int> numbers;
    long total = 0;
    // The consumer thread is the only code that touches total.
    std::thread consumer([&] {
        while (auto n = numbers.receive()) {
            total += *n;
        }
    });
    {
        std::vector<std::jthread> producers;
        for (int i = 0; i < 4; i++) {
            producers.emplace_back([&] {
                for (int k = 0; k < 1000; k++) {
                    numbers.send(1);
                }
            });
        }
    }  // The jthreads join here.
    numbers.close();
    consumer.join();
    std::println("total {}", total);

    // A string moved into the channel is transferred, not copied.
    Channel<std::string> texts;
    std::string s = "hello";
    texts.send(std::move(s));
    texts.close();
    std::println("{} {}", texts.receive().value_or("none"), texts.receive().has_value());
}
```

```output for=channel-from-mutex-and-condvar
total 4000
hello false
```

- Verdict: idiom. The standard library has no channel, so the example builds one from a `std::mutex`, a `std::condition_variable` and a `std::deque`. `receive` waits for a message and gives `std::nullopt` after `close`. Four `std::jthread` producers send 4000 messages to one consumer. A `std::string` moved into the channel is transferred, not copied.

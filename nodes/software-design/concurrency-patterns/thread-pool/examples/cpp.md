```cpp run id=thread-pool
#include <algorithm>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <deque>
#include <functional>
#include <future>
#include <iostream>
#include <mutex>
#include <set>
#include <thread>
#include <vector>

class ThreadPool {
public:
    explicit ThreadPool(int size) {
        for (int i = 0; i < size; i++) {
            threads_.emplace_back([this] {
                for (;;) {
                    std::function<void()> work;
                    {
                        std::unique_lock<std::mutex> lock(mutex_);
                        ready_.wait(lock, [this] { return stopping_ || !queue_.empty(); });
                        if (queue_.empty()) return;
                        work = std::move(queue_.front());
                        queue_.pop_front();
                    }
                    work();
                }
            });
        }
    }

    template <typename F>
    auto submit(F fn) -> std::future<decltype(fn())> {
        auto task = std::make_shared<std::packaged_task<decltype(fn())()>>(fn);
        {
            std::lock_guard<std::mutex> lock(mutex_);
            queue_.emplace_back([task] { (*task)(); });
        }
        ready_.notify_one();
        return task->get_future();
    }

    ~ThreadPool() {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            stopping_ = true;
        }
        ready_.notify_all();
        for (auto& t : threads_) t.join();
    }

private:
    std::vector<std::thread> threads_;
    std::deque<std::function<void()>> queue_;
    std::mutex mutex_;
    std::condition_variable ready_;
    bool stopping_ = false;
};

std::atomic<int> running{0};
std::atomic<int> most_running{0};
std::mutex workers_mutex;
std::set<std::thread::id> workers;

long long task(long long n) {
    int now = ++running;
    int seen = most_running.load();
    while (seen < now && !most_running.compare_exchange_weak(seen, now)) {
    }
    {
        std::lock_guard<std::mutex> lock(workers_mutex);
        workers.insert(std::this_thread::get_id());
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    --running;
    return n * n;
}

int main() {
    std::vector<std::future<long long>> futures;
    long long sum = 0;
    {
        ThreadPool pool(4);
        for (long long n = 0; n < 100; n++) {
            futures.push_back(pool.submit([n] { return task(n); }));
        }
        for (auto& f : futures) sum += f.get();
    }
    std::cout << "tasks run: " << futures.size() << "\n";
    std::cout << "sum of results: " << sum << "\n";
    std::cout << "workers used, at most 4: " << (workers.size() <= 4 ? "True" : "False") << "\n";
    std::cout << "most tasks running at once, at most 4: " << (most_running <= 4 ? "True" : "False") << "\n";
}
```

```output
tasks run: 100
sum of results: 328350
workers used, at most 4: True
most tasks running at once, at most 4: True
```

- Verdict: idiom. `ThreadPool` is a small class: 4 threads wait on a `condition_variable` for functions in a `deque`, and `submit` returns a `std::future` from a `std::packaged_task`.

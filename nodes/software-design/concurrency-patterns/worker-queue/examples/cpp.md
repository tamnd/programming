```cpp run id=worker-queue
#include <condition_variable>
#include <deque>
#include <iostream>
#include <map>
#include <mutex>
#include <optional>
#include <thread>
#include <utility>
#include <vector>

constexpr int kTasks = 10;
constexpr int kWorkers = 3;
constexpr int kFailsOn = 4;  // the first worker that takes this task fails before it does the work

// Hands out tasks and keeps each one until a worker acknowledges it.
class WorkQueue {
public:
    void put(int task) {
        std::lock_guard<std::mutex> lk(m_);
        ready_.push_back(task);
        pending_++;
        cv_.notify_all();
    }

    // Returns {tag, task}, or nothing when every task is acknowledged.
    std::optional<std::pair<int, int>> get() {
        std::unique_lock<std::mutex> lk(m_);
        cv_.wait(lk, [&] { return !ready_.empty() || pending_ == 0; });
        if (ready_.empty()) return std::nullopt;
        tag_++;
        deliveries_++;
        unacked_[tag_] = ready_.front();
        ready_.pop_front();
        return std::make_pair(tag_, unacked_[tag_]);
    }

    void ack(int tag) {
        std::lock_guard<std::mutex> lk(m_);
        unacked_.erase(tag);
        pending_--;
        cv_.notify_all();
    }

    // The worker went away without an ack, so the task goes back on the queue.
    void worker_lost(int tag) {
        std::lock_guard<std::mutex> lk(m_);
        ready_.push_back(unacked_[tag]);
        unacked_.erase(tag);
        put_back_++;
        cv_.notify_all();
    }

    int deliveries() { std::lock_guard<std::mutex> lk(m_); return deliveries_; }
    int put_back() { std::lock_guard<std::mutex> lk(m_); return put_back_; }
    int pending() { std::lock_guard<std::mutex> lk(m_); return pending_; }

private:
    std::mutex m_;
    std::condition_variable cv_;
    std::deque<int> ready_;
    std::map<int, int> unacked_;  // delivery tag -> task
    int pending_ = 0;             // tasks put and not yet acknowledged
    int tag_ = 0, deliveries_ = 0, put_back_ = 0;
};

int main() {
    WorkQueue work;
    for (int task = 1; task <= kTasks; task++) work.put(task);

    std::mutex mu;
    std::map<int, int> results;
    bool failed = false;
    std::vector<std::thread> threads;
    for (int i = 0; i < kWorkers; i++) {
        threads.emplace_back([&] {
            while (auto delivery = work.get()) {
                auto [tag, task] = *delivery;
                bool fail;
                {
                    std::lock_guard<std::mutex> lk(mu);
                    fail = task == kFailsOn && !failed;
                    if (fail) failed = true;
                }
                if (fail) {
                    work.worker_lost(tag);  // this worker stops here
                    return;
                }
                {
                    std::lock_guard<std::mutex> lk(mu);
                    results[task] = task * task;  // the work
                }
                work.ack(tag);  // only after the work is done
            }
        });
    }
    for (auto& t : threads) t.join();

    bool all = results.size() == kTasks && results.begin()->first == 1 && results.rbegin()->first == kTasks;
    std::cout << "tasks put on the queue: " << kTasks << "\n";
    std::cout << "workers: " << kWorkers << "\n";
    std::cout << "deliveries: " << work.deliveries() << "\n";
    std::cout << "tasks put back after a worker failed: " << work.put_back() << "\n";
    std::cout << "tasks still not acknowledged: " << work.pending() << "\n";
    std::cout << "every task has a result: " << (all ? "True" : "False") << "\n";
}
```

```output
tasks put on the queue: 10
workers: 3
deliveries: 11
tasks put back after a worker failed: 1
tasks still not acknowledged: 0
every task has a result: True
```

- Verdict: idiom. A `WorkQueue` class on `std::mutex` and `std::condition_variable`, with `std::optional` for the delivery.

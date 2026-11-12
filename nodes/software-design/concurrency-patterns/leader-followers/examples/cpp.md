```cpp run id=leader-followers
#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <deque>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

// Stands for a handle set that one thread at a time may wait on.
std::deque<int> handle_set;
std::mutex handle_mutex;

class ThreadPool {
public:
    void join() {
        while (true) {
            {
                std::unique_lock<std::mutex> lock(mutex_); // follower role
                synchronizer_.wait(lock, [this] { return !leader_present_; });
                leader_present_ = true;
                ++waiting_;
                most_waiting = std::max(most_waiting, waiting_);
            }
            int event;
            {
                std::lock_guard<std::mutex> lock(handle_mutex); // leader role
                event = handle_set.front();
                handle_set.pop_front();
            }
            promote_new_leader();
            if (event < 0) return;
            std::this_thread::sleep_for(std::chrono::milliseconds(1)); // processing role
            std::lock_guard<std::mutex> lock(mutex_);
            ++processed;
            sum += event;
        }
    }

    int most_waiting = 0;
    int processed = 0;
    long sum = 0;

private:
    void promote_new_leader() {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            --waiting_;
            leader_present_ = false;
        }
        synchronizer_.notify_one();
    }

    std::mutex mutex_;
    std::condition_variable synchronizer_;
    bool leader_present_ = false;
    int waiting_ = 0;
};

int main() {
    for (int id = 0; id < 100; ++id) handle_set.push_back(id);
    for (int i = 0; i < 4; ++i) handle_set.push_back(-1); // one stop event per thread
    ThreadPool pool;
    std::vector<std::thread> threads;
    for (int i = 0; i < 4; ++i) threads.emplace_back([&pool] { pool.join(); });
    for (auto& t : threads) t.join();
    std::cout << "events processed: " << pool.processed << "\n";
    std::cout << "sum of event ids: " << pool.sum << "\n";
    std::cout << "most threads waiting on the handle set at once: " << pool.most_waiting << "\n";
}
```

```output
events processed: 100
sum of event ids: 4950
most threads waiting on the handle set at once: 1
```

- Verdict: stdlib. A `std::condition_variable` is the synchronizer, and `notify_one` promotes one follower.

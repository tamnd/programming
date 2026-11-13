```cpp run id=three-ways
#include <condition_variable>
#include <deque>
#include <functional>
#include <iostream>
#include <mutex>
#include <numeric>
#include <thread>
#include <vector>

constexpr int kThreads = 4;
constexpr int kSteps = 1000;

void run_threads(const std::function<void(int)>& body) {
    std::vector<std::thread> threads;
    for (int i = 0; i < kThreads; i++) threads.emplace_back(body, i);
    for (auto& t : threads) t.join();
}

int main() {
    // 1. Synchronization: a mutex guards the shared counter.
    int count = 0;
    std::mutex m;
    run_threads([&](int) {
        for (int n = 0; n < kSteps; n++) {
            std::lock_guard<std::mutex> lk(m);
            count++;
        }
    });
    std::cout << "lock around a shared counter: " << count << "\n";

    // 2. Confinement: one owner thread keeps the counter; the others send it messages.
    std::mutex qm;
    std::condition_variable cv;
    std::deque<int> inbox;
    int owned = 0;
    std::thread owner([&] {
        int total = 0;
        for (int n = 0; n < kThreads * kSteps; n++) {
            std::unique_lock<std::mutex> lk(qm);
            cv.wait(lk, [&] { return !inbox.empty(); });
            total += inbox.front();
            inbox.pop_front();
        }
        owned = total;
    });
    run_threads([&](int) {
        for (int n = 0; n < kSteps; n++) {
            {
                std::lock_guard<std::mutex> lk(qm);
                inbox.push_back(1);
            }
            cv.notify_one();
        }
    });
    owner.join();  // join makes the owner's write visible here
    std::cout << "one owner thread, the others send messages: " << owned << "\n";

    // 3. No sharing: each thread makes its own result, and the results are combined after the join.
    std::vector<int> parts(kThreads, 0);
    run_threads([&](int i) {
        for (int n = 0; n < kSteps; n++) parts[i]++;  // only thread i writes slot i
    });
    std::cout << "own result for each thread, combined after the join: "
              << std::accumulate(parts.begin(), parts.end(), 0) << "\n";
}
```

```output
lock around a shared counter: 4000
one owner thread, the others send messages: 4000
own result for each thread, combined after the join: 4000
```

- Verdict: idiom. `std::mutex` with `std::lock_guard`, a queue that a mutex and a `std::condition_variable` protect, and a `std::vector` slot for each thread.

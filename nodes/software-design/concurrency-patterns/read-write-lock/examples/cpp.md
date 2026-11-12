```cpp run id=read-write-lock
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <vector>

std::shared_mutex rw;
int pair_[2] = {0, 0};  // the writer keeps the two values equal
std::atomic<int> inside{0}, reads{0}, mixed{0};

int main() {
    // Part 1: two readers hold the read lock at the same time.
    std::mutex m;
    std::condition_variable cv;
    int at_meeting = 0, met = 0;
    std::vector<std::thread> meets;
    for (int i = 0; i < 2; i++) {
        meets.emplace_back([&] {
            std::shared_lock<std::shared_mutex> read_lock(rw);
            std::unique_lock<std::mutex> lk(m);
            at_meeting++;
            cv.notify_all();
            if (cv.wait_for(lk, std::chrono::seconds(5), [&] { return at_meeting == 2; })) met++;
        });
    }
    for (auto& t : meets) t.join();

    // Part 2: readers and a writer share the pair.
    int writes = 0, overlap = 0;
    std::vector<std::thread> threads;
    for (int r = 0; r < 4; r++) {
        threads.emplace_back([] {
            for (int n = 0; n < 1000; n++) {
                std::shared_lock<std::shared_mutex> read_lock(rw);
                inside++;
                int x = pair_[0];
                std::this_thread::yield();  // give the writer a chance to run here
                int y = pair_[1];
                inside--;
                reads++;
                if (x != y) mixed++;
            }
        });
    }
    threads.emplace_back([&] {
        for (int i = 1; i <= 100; i++) {
            std::unique_lock<std::shared_mutex> write_lock(rw);
            if (inside > 0) overlap++;
            pair_[0] = i;
            std::this_thread::yield();
            pair_[1] = i;
            writes++;
        }
    });
    for (auto& t : threads) t.join();

    std::cout << "two readers inside at once: " << (met == 2 ? "True" : "False") << "\n";
    std::cout << "reads: " << reads << "\n";
    std::cout << "reads that saw a mixed pair: " << mixed << "\n";
    std::cout << "writes: " << writes << "\n";
    std::cout << "writes that ran with a reader inside: " << overlap << "\n";
}
```

```output
two readers inside at once: True
reads: 4000
reads that saw a mixed pair: 0
writes: 100
writes that ran with a reader inside: 0
```

- Verdict: stdlib. `std::shared_mutex` with `std::shared_lock` for readers and `std::unique_lock` for the writer.

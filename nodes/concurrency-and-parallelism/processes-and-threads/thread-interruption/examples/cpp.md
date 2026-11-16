```cpp run id=stop-a-thread
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <stop_token>
#include <thread>

int main() {
    using namespace std::chrono_literals;
    std::mutex m;
    std::condition_variable_any cv;
    std::chrono::steady_clock::time_point start;
    {
        std::jthread worker([&](std::stop_token st) {
            std::unique_lock lock(m);
            // Waits for up to 10 s, but a stop request ends the wait at once.
            cv.wait_for(lock, st, 10s, [] { return false; });
            std::cout << "worker: stop requested: " << std::boolalpha << st.stop_requested() << '\n';
        });
        std::this_thread::sleep_for(100ms);
        start = std::chrono::steady_clock::now();
        worker.request_stop();
    }  // The jthread destructor joins the thread.
    std::cout << "worker stopped in under 1 s: " << std::boolalpha
              << (std::chrono::steady_clock::now() - start < 1s) << '\n';
}
```

```output for=stop-a-thread
worker: stop requested: true
worker stopped in under 1 s: true
```

- Verdict: stdlib. `std::jthread` "can be cancelled/stopped in certain situations". It holds a `std::stop_source`, and it passes a `std::stop_token` to the thread function.
- `request_stop()` sets the stop state. The `std::stop_token` can be passed "to the interruptible waiting functions of std::condition_variable_any", so the 10-second wait ends at once.
- The jthread destructor joins the thread, so the worker has finished when the last line prints.
- "Unlike std::thread , the jthread logically holds an internal private member of type std::stop_source". Code that uses a plain `std::thread` needs its own flag, such as a `std::atomic<bool>`.

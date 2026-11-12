```cpp run id=tss
#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

thread_local int last_error = 0; // one name, one value per thread

int fake_call(int code) {
    last_error = code; // the call sets its thread's error
    std::this_thread::yield(); // let another thread run between the set and the test
    return -1;
}

int main() {
    std::atomic<int> wrong_reads{0};
    int seen[4] = {0, 0, 0, 0};
    std::vector<std::thread> threads;
    for (int index = 0; index < 4; ++index) {
        threads.emplace_back([&, index] {
            int code = index + 1;
            for (int n = 0; n < 1000; ++n) {
                if (fake_call(code) == -1 && last_error != code) ++wrong_reads;
            }
            seen[index] = last_error;
        });
    }
    for (auto& t : threads) t.join();
    std::cout << "threads: " << threads.size() << "\n";
    std::cout << "calls per thread: " << 1000 << "\n";
    std::cout << "reads that saw another thread's error: " << wrong_reads << "\n";
    std::cout << "error code seen by each thread: [" << seen[0] << ", " << seen[1] << ", " << seen[2]
              << ", " << seen[3] << "]\n";
}
```

```output
threads: 4
calls per thread: 1000
reads that saw another thread's error: 0
error code seen by each thread: [1, 2, 3, 4]
```

- Verdict: native. The `thread_local` keyword gives each thread its own `last_error`.

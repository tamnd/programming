```cpp run id=sums
#include <numeric>
#include <print>
#include <thread>
#include <vector>

int main() {
    std::vector<long> results(4);
    {
        std::vector<std::jthread> workers;
        for (int i = 0; i < 4; ++i) {
            workers.emplace_back([&results, i] {
                long sum = 0;
                for (long n = i * 250; n < (i + 1) * 250; ++n) {
                    sum += n;
                }
                results[i] = sum;
            });
        }
    }
    std::println("{}", results);
    std::println("total: {}", std::accumulate(results.begin(), results.end(), 0L));

    bool ran = false;
    std::thread worker([&ran] { ran = true; });
    std::println("joinable before join: {}", worker.joinable());
    worker.join();
    std::println("ran: {}, joinable after join: {}", ran, worker.joinable());
}
```

```output for=sums
[31125, 93625, 156125, 218625]
total: 499500
joinable before join: true
ran: true, joinable after join: false
```

- Verdict: stdlib. `std::thread` runs a callable on another thread. The end of the thread synchronizes with the return of `join()`, so `main` sees each result.
- `std::jthread` joins in its destructor. The block around the workers ends only when all four threads have ended.
- A `std::thread` that is still joinable when it is destroyed calls `std::terminate` (see Pitfalls). Call `join()` or `detach()` first.
- The callable cannot return a value through `join()`. Write to a captured location, or use `std::async` and a `std::future`.

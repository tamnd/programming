```cpp run id=future
#include <future>
#include <iostream>
#include <stdexcept>
#include <thread>

int main() {
    std::future<int> price = std::async(std::launch::async, [] { return 40; });
    std::cout << price.get() + 2 << "\n";

    std::promise<std::string> promise;
    std::future<std::string> result = promise.get_future();
    std::thread worker([&promise] { promise.set_value("done"); });
    std::cout << result.get() << "\n";
    worker.join();

    auto failing = std::async(std::launch::async, []() -> int { throw std::runtime_error("no stock"); });
    try {
        failing.get();
    } catch (const std::runtime_error& e) {
        std::cout << "caught: " << e.what() << "\n";
    }
}
```

```output for=future
42
done
caught: no stock
```

- Verdict: stdlib. `std::promise` stores a value or an exception once, and the `std::future` that it makes reads it once with `get`. `std::async` runs a callable and returns its future.
- `std::future` has no `then`, so a program cannot chain work without a blocked thread. `std::shared_future` lets many readers wait for the same value.

```cpp wrong id=async-blocks
#include <chrono>
#include <future>
#include <iostream>
#include <thread>

void slow_log() {
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    std::cout << "log written\n";
}

int main() {
    std::async(std::launch::async, slow_log);
    std::cout << "main continues\n";
}
```

```output for=async-blocks
log written
main continues
```

The future from `std::async` is a temporary. Its destructor waits until the task ends, so the call runs as if it were synchronous. Keep the future in a variable for as long as the work must run in the background.

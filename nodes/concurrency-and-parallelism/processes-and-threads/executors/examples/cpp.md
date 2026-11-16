```cpp run id=executor-policies
#include <future>
#include <iostream>
#include <thread>

int main() {
    const auto caller = std::this_thread::get_id();
    auto task = [] { return std::this_thread::get_id(); };

    // The launch policy chooses how the same task runs.
    auto deferred = std::async(std::launch::deferred, task);
    auto async = std::async(std::launch::async, task);

    std::cout << std::boolalpha;
    std::cout << "deferred: ran on the caller's thread: " << (deferred.get() == caller) << '\n';
    std::cout << "async: ran on the caller's thread: " << (async.get() == caller) << '\n';
}
```

```output for=executor-policies
deferred: ran on the caller's thread: true
async: ran on the caller's thread: false
```

- Verdict: absent. The C++ standard library has no executor type. `std::async` with a launch policy is the nearest tool.
- With `std::launch::deferred`, the task runs in the thread that calls `get`, when it calls `get`. With `std::launch::async`, it runs as if on a new thread.
- `std::async` with no policy lets the library choose between the two.

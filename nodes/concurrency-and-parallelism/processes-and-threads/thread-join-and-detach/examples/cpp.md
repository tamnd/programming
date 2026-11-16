```cpp run id=jthread
#include <chrono>
#include <iostream>
#include <stop_token>
#include <thread>

int main() {
    int answer = 0;
    std::thread worker([&answer] { answer = 6 * 7; });
    worker.join();
    std::cout << std::boolalpha;
    std::cout << "joined: answer = " << answer << ", joinable() = " << worker.joinable() << '\n';

    std::thread sleeper([] { std::this_thread::sleep_for(std::chrono::hours(1)); });
    sleeper.detach();
    std::cout << "detached: joinable() = " << sleeper.joinable() << '\n';

    {
        std::jthread poller([](std::stop_token token) {
            while (!token.stop_requested()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
            std::cout << "jthread saw the stop request\n";
        });
    }  // The destructor of poller requests a stop, then joins.
    std::cout << "main returns; the detached thread ends with the process\n";
}
```

```output for=jthread
joined: answer = 42, joinable() = false
detached: joinable() = false
jthread saw the stop request
main returns; the detached thread ends with the process
```

- Verdict: stdlib. `std::thread` has `join` and `detach`, and `std::jthread` (C++20) joins in its destructor.
- After `join` or `detach`, `joinable()` is `false`, and the object can be destroyed. If a joinable `std::thread` is destroyed, the program calls `std::terminate`.
- `join` makes the write to `answer` visible to the main thread, so the main thread reads it without a lock.
- The destructor of `poller` asks the thread to stop through its `std::stop_token` and then joins it. The thread checks the token and returns.

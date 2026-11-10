```cpp run id=load
#include <chrono>
#include <coroutine>
#include <exception>
#include <functional>
#include <print>
#include <queue>
#include <string>
#include <thread>
#include <vector>

using Clock = std::chrono::steady_clock;

// A coroutine type that starts at once and frees its frame at the end.
struct Detached {
    struct promise_type {
        Detached get_return_object() { return {}; }
        std::suspend_never initial_suspend() { return {}; }
        std::suspend_never final_suspend() noexcept { return {}; }
        void return_void() {}
        void unhandled_exception() { std::terminate(); }
    };
};

// The event loop: suspended coroutines that wait for a time, earliest first.
struct Timer {
    Clock::time_point due;
    std::coroutine_handle<> handle;
    bool operator>(const Timer &other) const { return due > other.due; }
};
std::priority_queue<Timer, std::vector<Timer>, std::greater<>> timers;

// co_await Sleep{...} suspends the coroutine and adds a timer.
struct Sleep {
    std::chrono::milliseconds delay;
    bool await_ready() const { return false; }
    void await_suspend(std::coroutine_handle<> h) const {
        timers.push({Clock::now() + delay, h});
    }
    void await_resume() const {}
};

Detached load(std::string name, int ms, std::string &result) {
    std::println("start {}", name);
    co_await Sleep{std::chrono::milliseconds(ms)};
    std::println("end {}", name);
    result = name;
}

int main() {
    std::vector<std::string> results(2);
    load("slow", 200, results[0]);
    load("fast", 100, results[1]);
    while (!timers.empty()) {
        Timer next = timers.top();
        timers.pop();
        std::this_thread::sleep_until(next.due);
        next.handle.resume();
    }
    std::println("{}, {}", results[0], results[1]);
}
```

```output for=load
start slow
start fast
end fast
end slow
slow, fast
```

- Verdict: native. A function that contains `co_await`, `co_yield` or `co_return` is a coroutine.
- The language gives the mechanism, not a task type or a scheduler. The return type names a promise type, and the promise type says whether the coroutine starts at once (`initial_suspend`) and what happens at the end.
- This example defines the coroutine type `Detached`, the awaitable `Sleep` and a timer queue as the event loop. Programs usually take these from a library. The C++26 working draft adds `std::execution::task`.
- `Detached` starts at once and frees its state at the end. The results go into strings that live in `main`, which outlive the coroutines.

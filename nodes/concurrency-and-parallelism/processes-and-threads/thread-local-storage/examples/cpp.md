```cpp run id=per-thread-values
#include <condition_variable>
#include <deque>
#include <functional>
#include <iostream>
#include <mutex>
#include <optional>
#include <string>
#include <thread>

// Each thread gets its own counter, which starts at 0.
thread_local int counter = 0;
thread_local std::optional<std::string> user;

int bump(int times) {
    for (int i = 0; i < times; ++i) ++counter;
    return counter;
}

int main() {
    int one = 0;
    int two = 0;
    {
        std::jthread t1([&] { one = bump(1); });
        std::jthread t2([&] { two = bump(2); });
    }  // the jthreads join here
    std::cout << "main: counter = " << bump(3) << '\n';
    std::cout << "thread 1: counter = " << one << '\n';
    std::cout << "thread 2: counter = " << two << '\n';

    // A worker thread that runs one task after another, as a pool thread does.
    std::deque<std::function<void()>> tasks{
        [] { user = "alice"; },
        [] { std::cout << "a later task on the same thread sees user: " << user.value_or("nullopt") << '\n'; },
        [] { user.reset(); },
        [] { std::cout << "after a task removes it, the next task sees user: " << user.value_or("nullopt") << '\n'; },
    };
    std::jthread worker([&] {
        for (auto& task : tasks) task();
    });
}
```

```output for=per-thread-values
main: counter = 3
thread 1: counter = 1
thread 2: counter = 2
a later task on the same thread sees user: alice
after a task removes it, the next task sees user: nullopt
```

- Verdict: native. A variable declared `thread_local` has thread storage duration: each thread has its own object, which lasts as long as the thread.
- `std::jthread` joins in its destructor, so the closing brace waits for both threads.
- The worker thread runs four tasks in order, as a pool thread does. The value that the first task sets stays until a task resets it.
- C has the same feature: `_Thread_local` in C11, and `thread_local` as a keyword in C23.

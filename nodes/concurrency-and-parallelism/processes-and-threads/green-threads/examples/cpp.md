```cpp run id=ucontext
#include <ucontext.h>

#include <array>
#include <iostream>

namespace {

constexpr int kThreads = 2;
constexpr std::array<int, kThreads> kSteps{3, 2};

ucontext_t scheduler;
std::array<ucontext_t, kThreads> green;
std::array<bool, kThreads> finished{};
int current = 0;

// Save this green thread and switch back to the scheduler.
void yield() { swapcontext(&green[current], &scheduler); }

void worker() {
    const int id = current;
    for (int i = 0; i < kSteps[id]; ++i) {
        std::cout << static_cast<char>('a' + id) << " step " << i << '\n';
        yield();
    }
    finished[id] = true;
}  // returning resumes uc_link, which is the scheduler

}  // namespace

int main() {
    static std::array<std::array<char, 64 * 1024>, kThreads> stacks;
    for (int id = 0; id < kThreads; ++id) {
        getcontext(&green[id]);
        green[id].uc_stack.ss_sp = stacks[id].data();
        green[id].uc_stack.ss_size = stacks[id].size();
        green[id].uc_link = &scheduler;
        makecontext(&green[id], worker, 0);
    }
    int alive = kThreads;
    while (alive > 0) {
        for (current = 0; current < kThreads; ++current) {
            if (finished[current]) {
                continue;
            }
            swapcontext(&scheduler, &green[current]);
            if (finished[current]) {
                --alive;
            }
        }
    }
}
```

```output for=ucontext
a step 0
b step 0
a step 1
b step 1
a step 2
```

- Verdict: idiom. The C++ standard library has no green threads. On POSIX systems, the C calls `makecontext` and `swapcontext` give each green thread its own stack, and a loop can schedule them.
- `makecontext` sets a context to start `worker` on its own 64 KiB stack. `swapcontext` saves the current registers and stack pointer and switches to another context.
- `yield` switches back to the scheduler. When `worker` returns, the context in `uc_link` resumes, which is the scheduler again.
- POSIX.1-2008 does not include these calls, but glibc still has them. C++20 coroutines are the stackless alternative in the standard.

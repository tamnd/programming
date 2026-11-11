```cpp run id=decorrelated
#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>

using std::chrono::milliseconds;

struct FakeClock {
    milliseconds now{0};
    void sleep(milliseconds d) { now += d; }
};

int main() {
    const milliseconds base{100}, cap{2000};
    std::mt19937 rng(2024);
    FakeClock clock;

    milliseconds sleep = base;
    for (int attempt = 1; attempt <= 8; ++attempt) {
        std::uniform_int_distribution<long long> pick(base.count(), sleep.count() * 3);
        sleep = std::min(cap, milliseconds{pick(rng)});
        std::cout << "t=" << clock.now.count() << " ms: attempt " << attempt
                  << " failed; wait " << sleep.count() << " ms\n";
        clock.sleep(sleep);
    }
    std::cout << "gave up at t=" << clock.now.count() << " ms\n";
}
```

```output
t=0 ms: attempt 1 failed; wait 236 ms
t=236 ms: attempt 2 failed; wait 606 ms
t=842 ms: attempt 3 failed; wait 708 ms
t=1550 ms: attempt 4 failed; wait 740 ms
t=2290 ms: attempt 5 failed; wait 1672 ms
t=3962 ms: attempt 6 failed; wait 2000 ms
t=5962 ms: attempt 7 failed; wait 283 ms
t=6245 ms: attempt 8 failed; wait 227 ms
gave up at t=6472 ms
```

- Verdict: stdlib. C++ has no retry facility. `<random>` gives a seeded generator, and `<chrono>` gives typed durations, so a delay cannot be mixed up with a count.
- This is the "decorrelated jitter" of the AWS article: each sleep is a random value between the base and three times the previous sleep, capped. The delays grow on average but do not follow a fixed curve.
- The output is the same on each run with one standard library. `std::mt19937` is fully specified, but the algorithm of `uniform_int_distribution` is left to the implementation.

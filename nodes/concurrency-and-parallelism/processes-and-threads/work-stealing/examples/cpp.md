```cpp run id=work-stealing-model
#include <algorithm>
#include <array>
#include <deque>
#include <iostream>
#include <string>
#include <utility>

// A step-by-step model of a work-stealing scheduler with two workers.
// Each worker has a deque. The front is the top (oldest task); the back is the bottom (newest task).
// A task is a range [lo, hi). A range of more than 2 numbers splits into two halves; a smaller range is a leaf that adds its numbers.
int main() {
    constexpr int workers = 2;
    std::array<std::deque<std::pair<int, int>>, workers> deques;
    std::array<int, workers> leaves{};
    int total = 0;

    deques[0].push_back({0, 8});
    auto busy = [&] {
        return std::any_of(deques.begin(), deques.end(), [](const auto& d) { return !d.empty(); });
    };
    for (int round = 1; busy(); ++round) {
        for (int w = 0; w < workers; ++w) {
            std::pair<int, int> task;
            std::string how = "pops";
            std::string source;
            if (!deques[w].empty()) {
                task = deques[w].back();  // the owner takes the newest task (LIFO)
                deques[w].pop_back();
            } else {
                int victim = -1;
                for (int v = 0; v < workers; ++v) {
                    if (v != w && !deques[v].empty()) {
                        victim = v;
                        break;
                    }
                }
                if (victim == -1) {
                    std::cout << "round " << round << ": w" << w << " is idle\n";
                    continue;
                }
                task = deques[victim].front();  // a thief takes the oldest task (FIFO)
                deques[victim].pop_front();
                how = "steals";
                source = " from w" + std::to_string(victim);
            }
            auto [lo, hi] = task;
            std::cout << "round " << round << ": w" << w << ' ' << how << " [" << lo << ", " << hi << ')'
                      << source;
            if (hi - lo > 2) {
                int mid = (lo + hi) / 2;
                deques[w].push_back({lo, mid});
                deques[w].push_back({mid, hi});
                std::cout << " and splits it\n";
            } else {
                for (int i = lo; i < hi; ++i) total += i;
                ++leaves[w];
                std::cout << " and runs it\n";
            }
        }
    }

    std::cout << "leaves run: w0 " << leaves[0] << ", w1 " << leaves[1] << '\n';
    std::cout << "sum: " << total << '\n';
}
```

```output for=work-stealing-model
round 1: w0 pops [0, 8) and splits it
round 1: w1 steals [0, 4) from w0 and splits it
round 2: w0 pops [4, 8) and splits it
round 2: w1 pops [2, 4) and runs it
round 3: w0 pops [6, 8) and runs it
round 3: w1 pops [0, 2) and runs it
round 4: w0 pops [4, 6) and runs it
round 4: w1 is idle
leaves run: w0 2, w1 2
sum: 28
```

- Verdict: library. The C++ standard library has no work-stealing pool. Intel Threading Building Blocks has a work-stealing scheduler.
- This file is a model of the rules, with two `std::deque` objects in place of two threads. `pop_back` is the owner's LIFO pop, and `pop_front` is the thief's FIFO steal.
- A real scheduler must make the two ends safe to use from two threads at the same time. That is the hard part of a work-stealing deque, and a model on one thread does not show it.

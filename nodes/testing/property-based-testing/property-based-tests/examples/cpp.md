```cpp run id=checker
#include <charconv>
#include <cstdint>
#include <limits>
#include <optional>
#include <print>
#include <string>
#include <utility>
#include <vector>

// xorshift32 with Marsaglia's seed: the same numbers on every run.
struct Rng {
    std::uint32_t state = 2463534242u;

    std::uint32_t next() {
        state ^= state << 13;
        state ^= state >> 17;
        state ^= state << 5;
        return state;
    }

    std::int64_t between(std::int64_t lo, std::int64_t hi) {
        return lo + static_cast<std::int64_t>(next() % static_cast<std::uint64_t>(hi - lo + 1));
    }
};

// Runs a property on 100 generated cases and shrinks the first failure.
template <typename Generate, typename Holds, typename Shrink, typename Show>
void check(const char* name, Generate generate, Holds holds, Shrink shrink, Show show) {
    Rng rng;
    for (int trial = 1; trial <= 100; ++trial) {
        auto c = generate(rng);
        if (holds(c)) {
            continue;
        }
        std::println("{}: falsified on test {} by {}", name, trial, show(c));
        int steps = 0;
        for (bool shrunk = true; shrunk;) {
            shrunk = false;
            for (const auto& smaller : shrink(c)) {
                if (!holds(smaller)) {
                    c = smaller;
                    shrunk = true;
                    ++steps;
                    break;
                }
            }
        }
        std::println("{}: shrunk in {} steps to {}", name, steps, show(c));
        return;
    }
    std::println("{}: passed 100 tests", name);
}

// Gives lo, then x - d for d = (x - lo) / 2, / 4, ..., 1.
std::vector<int> shrink_int(int x, int lo) {
    std::vector<int> out;
    if (x > lo) {
        out.push_back(lo);
    }
    for (int d = (x - lo) / 2; d > 0; d /= 2) {
        out.push_back(x - d);
    }
    return out;
}

int pages(int items, int per_page) {
    return items / per_page;  // bug: drops the last page when it is not full
}

int main() {
    using Limits = std::numeric_limits<std::int32_t>;
    check(
        "round trip",
        [](Rng& rng) { return static_cast<std::int32_t>(rng.between(Limits::min(), Limits::max())); },
        [](std::int32_t x) {
            std::string text = std::to_string(x);
            std::int32_t back = 0;
            auto [end, error] = std::from_chars(text.data(), text.data() + text.size(), back);
            return error == std::errc{} && back == x;
        },
        [](std::int32_t) { return std::vector<std::int32_t>{}; },
        [](std::int32_t x) { return std::to_string(x); });

    using Pair = std::pair<int, int>;
    check(
        "pages",
        [](Rng& rng) {
            int items = static_cast<int>(rng.between(0, 1000));
            int per_page = static_cast<int>(rng.between(1, 50));
            return Pair{items, per_page};
        },
        [](const Pair& p) { return pages(p.first, p.second) * p.second >= p.first; },
        [](const Pair& p) {
            std::vector<Pair> out;
            for (int n : shrink_int(p.first, 0)) out.emplace_back(n, p.second);
            for (int n : shrink_int(p.second, 1)) out.emplace_back(p.first, n);
            return out;
        },
        [](const Pair& p) { return std::format("({}, {})", p.first, p.second); });
}
```

```output
round trip: passed 100 tests
pages: falsified on test 1 by (967, 7)
pages: shrunk in 13 steps to (1, 2)
```

- Verdict: idiom. The C++ standard library has `<random>` with seeded engines, but no property checker.
- `check` is a function template. Each lambda has its own type, so the template takes the generator, the property, the shrinker and the printer with no `std::function` and no heap allocation.
- `std::from_chars` parses with no locale and no exceptions. It returns an error code, and the round trip tests it with the value.
- The engines in `<random>`, such as `std::mt19937`, give the same sequence for a seed on every platform. The distributions, such as `std::uniform_int_distribution`, do not have a fixed algorithm, so the example uses its own `between`.

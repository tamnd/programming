```cpp run id=templates
#include <array>
#include <cstddef>
#include <print>
#include <span>
#include <string_view>
#include <vector>

enum class Mutant { None, GtToGe, GtToLt, LeftToBest, StartAt0 };

// Each mutant is a separate instantiation; if constexpr drops the other branches.
template <Mutant M>
std::size_t argmax(std::span<const int> xs) {
    std::size_t best = 0;
    for (std::size_t i = M == Mutant::StartAt0 ? 0 : 1; i < xs.size(); ++i) {
        int left = M == Mutant::LeftToBest ? xs[best] : xs[i];
        bool bigger;
        if constexpr (M == Mutant::GtToGe) {
            bigger = left >= xs[best];
        } else if constexpr (M == Mutant::GtToLt) {
            bigger = left < xs[best];
        } else {
            bigger = left > xs[best];
        }
        if (bigger) {
            best = i;
        }
    }
    return best;
}

struct Case {
    std::vector<int> xs;
    std::size_t want;
};

template <Mutant M>
bool passes(std::span<const Case> suite) {
    for (const Case& c : suite) {
        if (argmax<M>(c.xs) != c.want) {
            return false;
        }
    }
    return true;
}

int main() {
    const std::vector<Case> strong{{{1, 2, 3}, 2}, {{1, 3, 2}, 1}, {{3, 1, 2}, 0}, {{2, 2, 1}, 0}};
    const std::span<const Case> weak = std::span(strong).first(3);
    if (!passes<Mutant::None>(strong)) {
        std::println("the original must pass first");
        return 1;
    }

    struct Row {
        std::string_view name;
        bool (*passes)(std::span<const Case>);
    };
    const std::array<Row, 4> rows{{
        {"> to >=", passes<Mutant::GtToGe>},
        {"> to <", passes<Mutant::GtToLt>},
        {"xs[i] to xs[best]", passes<Mutant::LeftToBest>},
        {"start at 0", passes<Mutant::StartAt0>},
    }};
    std::println("{:<20}{:<8}4 tests", "mutant", "3 tests");
    int weak_killed = 0, strong_killed = 0;
    for (const Row& row : rows) {
        const bool weak_dead = !row.passes(weak), strong_dead = !row.passes(strong);
        weak_killed += weak_dead;
        strong_killed += strong_dead;
        std::println("{:<20}{:<8}{}", row.name, weak_dead ? "killed" : "lived", strong_dead ? "killed" : "lived");
    }
    std::println("{:<20}{} of 4  {} of 4", "killed", weak_killed, strong_killed);
}
```

```output
mutant              3 tests 4 tests
> to >=             lived   killed
> to <              killed  killed
xs[i] to xs[best]   killed  killed
start at 0          lived   lived
killed              2 of 4  3 of 4
```

- Verdict: library. The C++ standard library has no mutation tool. Mull mutates C and C++ programs through LLVM.
- The mutant is a template argument, not a run-time value. `argmax<Mutant::GtToGe>` and `argmax<Mutant::None>` are two different functions, and `if constexpr` drops the branches that do not apply. This is close to what a tool does when it builds each mutant as a separate program, but the compiler runs only once.
- `passes<Mutant::GtToGe>` is an ordinary function, so its address fits in the table `rows`. The loop then treats the mutants as data.
- `std::span(strong).first(3)` is the weak suite: a view of the first three cases, with no copy.

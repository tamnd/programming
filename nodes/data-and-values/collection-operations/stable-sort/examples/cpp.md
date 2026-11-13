```cpp run id=stable-sort-projection
#include <algorithm>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

struct Staff {
    std::string name, team;
    int grade;
};

static void show(const std::vector<Staff> &v) {
    for (const auto &p : v) std::cout << p.name << ' ';
    std::cout << '\n';
}

int main() {
    const std::vector<Staff> all{{"ann", "ops", 3}, {"bob", "dev", 2}, {"cy", "ops", 2}, {"di", "dev", 3}, {"ed", "ops", 2}};

    // std::stable_sort keeps equal teams in the name order.
    auto s = all;
    std::ranges::stable_sort(s, {}, &Staff::team);
    show(s);

    // Two stable passes: grade first, then team.
    s = all;
    std::ranges::stable_sort(s, {}, &Staff::grade);
    std::ranges::stable_sort(s, {}, &Staff::team);
    show(s);

    // std::sort is not stable. A comparator on every field leaves no ties.
    s = all;
    std::ranges::sort(s, [](const Staff &a, const Staff &b) {
        return std::tie(a.team, a.grade, a.name) < std::tie(b.team, b.grade, b.name);
    });
    show(s);
}
```

```output
bob di ann cy ed
bob di cy ed ann
bob di cy ed ann
```

- Verdict: native. `std::ranges::stable_sort` with the projection `&Staff::team` keeps the name order inside each team. Two stable passes sort by team and then by grade. `std::ranges::sort` is not stable, so its comparator uses `std::tie` on every field.

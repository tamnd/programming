```cpp run id=bool-comparator-and-spaceship
#include <algorithm>
#include <compare>
#include <functional>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

struct Person {
    std::string name;
    int age;
    // A defaulted <=> compares the members in order: name, then age.
    auto operator<=>(const Person &) const = default;
};

static void show(const std::vector<Person> &v) {
    for (const auto &p : v) std::cout << p.name << ' ';
    std::cout << '\n';
}

int main() {
    std::vector<Person> people{{"cy", 31}, {"ann", 31}, {"bob", 25}};

    // A C++ comparator returns bool: true when the first argument goes first.
    auto byAge = [](const Person &a, const Person &b) { return a.age < b.age; };
    std::ranges::stable_sort(people, byAge);
    show(people);

    // std::tie compares several keys in order.
    std::ranges::sort(people, [](const Person &a, const Person &b) {
        return std::tie(a.age, a.name) < std::tie(b.age, b.name);
    });
    show(people);

    // A projection gives a key; std::ranges::greater reverses the order.
    std::ranges::sort(people, std::ranges::greater{}, &Person::age);
    std::cout << people.front().age << ' ' << people.back().age << '\n';

    // The defaulted <=> gives the natural order of the struct.
    std::ranges::sort(people);
    show(people);

    // <=> returns an ordering object, not a number.
    auto r = 1 <=> 2;
    std::cout << (r < 0) << ' ' << (r == std::strong_ordering::less) << '\n';
}
```

```output
bob cy ann
bob ann cy
31 25
ann bob cy
1 1
```

- Verdict: native. A C++ comparator returns `bool`. `stable_sort` by age keeps `cy` before `ann`. `std::tie` compares the age and then the name. A projection `&Person::age` with `std::ranges::greater` sorts by age in descending order. A defaulted `<=>` gives the natural order by name, and `1 <=> 2` is an ordering object that compares less than zero.

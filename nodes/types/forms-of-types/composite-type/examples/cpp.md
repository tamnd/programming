```cpp run id=tuple-pair-array
#include <array>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

struct Person {
    std::string name;
    int age;
    bool operator==(const Person&) const = default;
};

int main() {
    std::pair<int, std::string> pr{1, "a"};
    std::tuple<int, double, char> tp{2, 3.5, 'x'};
    std::cout << pr.second << ' ' << std::get<1>(tp) << ' ' << std::tuple_size_v<decltype(tp)> << '\n';

    auto [n, s] = pr;  // a structured binding takes the pair apart
    std::cout << n << s << '\n';

    std::array<int, 3> a{1, 2, 3};
    auto b = a;  // std::array copies like a value
    b[0] = 9;
    std::cout << a[0] << ' ' << (a == std::array<int, 3>{1, 2, 3}) << '\n';

    Person p{"Ann", 31};
    Person q = p;
    q.age = 32;
    std::cout << p.age << ' ' << (p == Person{"Ann", 31}) << '\n';

    std::vector<Person> people{p, q};
    std::cout << people.size() << ' ' << people[1].age << '\n';
}
```

```output for=tuple-pair-array
a 3.5 3
1a
1 1
31 1
2 32
```

- Verdict: native. `std::pair` and `std::tuple` are tuples from the library, and `std::get` and a structured binding take them apart. `std::array` is a fixed-size array that copies and compares like a value, unlike a built-in array. A struct copies its members on assignment, and a defaulted `operator==` compares them one by one. `std::vector<Person>` holds the records themselves, not pointers to them.

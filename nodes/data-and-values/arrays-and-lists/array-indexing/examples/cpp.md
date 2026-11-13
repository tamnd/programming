```cpp run id=at-and-operator-brackets
#include <array>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

int main() {
    std::vector<int> v{10, 20, 30};
    // operator[] does not check the index. at() checks it and throws std::out_of_range.
    std::cout << v[0] << ' ' << v.at(2) << ' ' << v.back() << '\n';
    try {
        std::cout << v.at(3) << '\n';
    } catch (const std::out_of_range& e) {
        std::cout << "out_of_range\n";
    }

    // size() is unsigned. ssize gives a signed size, so a loop that counts down can test i >= 0.
    for (auto i = std::ssize(v) - 1; i >= 0; --i) std::cout << v[i] << ' ';
    std::cout << '\n';

    // std::array has a size known at compile time, and std::get checks the index at compile time.
    std::array<int, 3> a{1, 2, 3};
    std::cout << std::get<2>(a) << ' ' << a.size() << '\n';

    // A string index gives a char, which is one byte.
    std::string s = "héllo";
    std::cout << s.size() << ' ' << s[0] << '\n';
}
```

```output
10 30 30
out_of_range
30 20 10
3 3
6 h
```

- Verdict: native. operator[] does not check the index, and at() throws std::out_of_range. std::ssize gives a signed size, so a count down can test i >= 0. std::get on a std::array checks the index at compile time. A string index gives one byte.

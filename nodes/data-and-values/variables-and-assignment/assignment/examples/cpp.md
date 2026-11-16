```cpp run id=operator-assign
#include <iostream>
#include <string>
#include <utility>

int main() {
    std::string a = "left";
    std::string b = "right";
    std::string c;
    c = a = b;
    std::cout << a << ' ' << c << '\n';
    std::pair<int, int> p{1, 2};
    p = {p.second, p.first};
    std::cout << p.first << ' ' << p.second << '\n';
}
```

```output for=operator-assign
right right
2 1
```

- Verdict: native. As in C, `=` is an expression and chains from the right. For a class type such as `std::string`, `=` calls the assignment operator of the class, which copies the characters. The braces on the right of `p = {...}` build a new pair from the old values before the assignment.

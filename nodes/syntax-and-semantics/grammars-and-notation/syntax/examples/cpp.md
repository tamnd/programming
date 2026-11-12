```cpp run id=most-vexing-parse
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

struct Timer {
    Timer() { std::cout << "Timer made\n"; }
};

int main() {
    // The most vexing parse: this declares a function, not an object.
    Timer t1();
    std::cout << std::is_function_v<decltype(t1)> << "\n";
    Timer t2{};  // braces make an object

    // >> closes two template argument lists.
    std::vector<std::vector<int>> grid{{1, 2}, {3}};
    std::cout << grid.size() << " " << grid[0].size() << "\n";

    // A range for is sugar for a loop with begin and end.
    int sum = 0;
    for (int v : grid[0]) sum += v;
    int sum2 = 0;
    for (auto it = grid[0].begin(), end = grid[0].end(); it != end; ++it) sum2 += *it;
    std::cout << sum << " " << sum2 << "\n";

    // Raw string literals change the syntax of a string: no escapes inside.
    std::string path = R"(C:\temp\new)";
    std::cout << path << "\n";
}
```

```output for=most-vexing-parse
1
Timer made
2 2
3 3
C:\temp\new
```

- Verdict: native. `Timer t1();` declares a function, which is the most vexing parse, so no object is made and the compiler warns about it on purpose. `Timer t2{};` makes an object. `>>` closes two template argument lists. A range `for` is sugar for a loop with `begin` and `end`. A raw string literal has no escapes.

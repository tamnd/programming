```cpp run id=digit-separators-and-raw-strings
#include <iostream>
#include <vector>

int main() {
    int i = 1, j = 10;
    int k = i+++j; // lexed as i ++ + j
    std::cout << k << ' ' << i << '\n';

    std::cout << 1'000'000 << ' ' << 0b1010 << ' ' << 017 << ' ' << 0xff << '\n';
    const char *raw = R"(He said "hi" to C:\temp)"; // a raw string
    std::cout << raw << '\n';
    std::vector<std::vector<int>> v{{1, 2}}; // >> closes two lists since C++11
    std::cout << v[0].size() << '\n';
    int x = 10, y = 3;
    std::cout << (x<y and y not_eq 0) << '\n'; // and and not_eq are tokens too
}
```

```output for=digit-separators-and-raw-strings
11 2
1000000 10 15 255
He said "hi" to C:\temp
2
0
```

- Verdict: absent. C++ has no standard library for its tokens; tools use Clang's libraries. C++ forms tokens as C does, by the longest match, and adds the apostrophe as a digit separator and raw strings in the form `R"(...)"`. Since C++11, `>>` closes two template argument lists. The alternative tokens `and`, `or` and `not_eq` are keywords that mean `&&`, `||` and `!=`.

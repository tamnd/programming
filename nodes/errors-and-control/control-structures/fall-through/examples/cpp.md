```cpp run id=fall-through-cpp
#include <iostream>
#include <string>

std::string rights(int level) {
    std::string out;
    switch (level) {
    case 3:
        out += " delete";
        [[fallthrough]];
    case 2:
        out += " write";
        [[fallthrough]];
    case 1:
        out += " read";
        break;
    default:
        out += " none";
        break;
    }
    return out;
}

int main() {
    std::cout << 3 << ':' << rights(3) << '\n';
    std::cout << 2 << ':' << rights(2) << '\n';
    std::cout << 0 << ':' << rights(0) << '\n';
}
```

```output for=fall-through-cpp
3: delete write read
2: write read
0: none
```

- Verdict: native. A C++ case without `break` falls through. The C++17 statement `[[fallthrough]];` marks an intentional fall-through; it must come just before a `case` or `default` label.

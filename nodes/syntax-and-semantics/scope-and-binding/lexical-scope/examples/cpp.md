```cpp run id=scopes
#include <iostream>
#include <string>

namespace config {
std::string level = "namespace";
}

std::string reads_namespace() { return config::level; }

std::string caller() {
    std::string level = "caller";
    return reads_namespace() + ", local " + level;
}

int main() {
    std::cout << caller() << "\n";
    if (int n = 3; n > 2) {
        std::cout << "n in the if: " << n << "\n";
    }
    int total = 0;
    for (int i = 0; i < 3; ++i) total += i;
    std::cout << "total: " << total << "\n";
}
```

```output for=scopes
namespace, local caller
n in the if: 3
total: 3
```

- Verdict: native. C++ has the block scope of C, plus namespace scope, class scope and the scope of the init-statement in `if`, `switch` and `for`. The `n` in the `if` header is not visible after the statement.
- A name in an inner scope hides the same name in an outer scope. A qualified name, such as `config::level` or `this->x`, reaches the hidden entity.

```cpp wrong id=self-assign
#include <iostream>

struct Point {
    int x = 0;
    int y = 0;

    Point(int x, int y) {
        x = x;
        this->y = y;
    }
};

int main() {
    Point p(3, 4);
    std::cout << p.x << " " << p.y << "\n";
}
```

```output for=self-assign
0 4
```

In the constructor body, the parameter `x` hides the member `x`, so `x = x` assigns the parameter to itself. The member keeps `0`. Use a member initializer list, `Point(int x, int y) : x(x), y(y) {}`, where the name before the parentheses is always the member.

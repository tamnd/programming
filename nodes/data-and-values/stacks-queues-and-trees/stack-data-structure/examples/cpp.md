```cpp run id=stack
#include <initializer_list>
#include <iostream>
#include <stack>
#include <string>

int main() {
    std::stack<std::string> stack;
    for (const char *item : {"a", "b", "c"}) {
        stack.push(item);
    }
    std::cout << "size " << stack.size() << " top " << stack.top() << '\n';

    // pop returns void, so read top first.
    std::string first = stack.top();
    stack.pop();
    std::string second = stack.top();
    stack.pop();
    std::cout << first << ' ' << second << " left " << stack.size() << '\n';

    stack.pop();
    std::cout << "empty: " << (stack.empty() ? "yes" : "no") << '\n';
}
```

```output for=stack
size 3 top c
c b left 1
empty: yes
```

- Verdict: stdlib. `std::stack` is an adaptor over `std::deque` by default. It gives `push`, `pop`, `top`, `size` and `empty`, and no iteration.
- `pop` returns nothing, so the program reads `top()` before each `pop()`.
- The program checks `empty()` first, because `top` or `pop` on an empty stack is not allowed.

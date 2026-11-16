```cpp run id=strict
#include <functional>
#include <iostream>
#include <string>

int trace(const std::string& label, int value) {
    std::cout << "evaluating " << label << '\n';
    return value;
}

int twice(int n) {
    std::cout << "inside twice\n";
    return 2 * n;
}

int choose(bool flag, const std::function<int()>& left, const std::function<int()>& right) {
    return flag ? left() : right();
}

int main() {
    std::cout << twice(trace("n", 21)) << '\n';
    std::cout << choose(false, [] { return trace("left", 1); }, [] { return trace("right", 2); }) << '\n';
}
```

```output for=strict
evaluating n
inside twice
42
evaluating right
2
```

- Verdict: native. Every argument expression is sequenced before the body of the function. The arguments are indeterminately sequenced among themselves, so the compiler chooses their order.
- A lambda with no parameters is a thunk. `choose` calls only the lambda that it needs, so `left` never runs.

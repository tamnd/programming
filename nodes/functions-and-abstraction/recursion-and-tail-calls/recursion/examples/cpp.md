```cpp run id=tree
#include <print>
#include <vector>

struct Node {
    int value;
    std::vector<Node> children;
};

int total(const Node& node) {
    int sum = node.value;
    for (const Node& child : node.children) {
        sum += total(child);
    }
    return sum;
}

constexpr long factorial(int n) { return n == 0 ? 1 : n * factorial(n - 1); }

static_assert(factorial(10) == 3628800);

int main() {
    Node tree{1, {Node{2, {Node{4, {}}, Node{5, {}}}}, Node{3, {}}}};
    std::println("{}", total(tree));
    std::println("{}", factorial(20));
}
```

```output for=tree
15
2432902008176640000
```

- Verdict: native. Functions can call themselves, and a `constexpr` function can recurse during constant evaluation, so `static_assert` checks `factorial(10)` at compile time.
- The compiler limits the depth of constant evaluation. GCC sets it with `-fconstexpr-depth`, and the standard asks for at least 512.
- A local object with a destructor runs code after the recursive call returns, so the call is not in tail position. Clang's `musttail` attribute requires that the variables in scope be trivially destructible.
- `std::vector<Node>` inside `Node` gives a recursive type. The vector holds its elements on the heap, so the size of `Node` is finite.

Endless recursion in a `constexpr` evaluation is a compile error, not a crash at run time.

```cpp fails expect=compile id=constexpr-limit
constexpr int forever(int n) { return forever(n + 1); }

static_assert(forever(0) == 0);

int main() {}
```

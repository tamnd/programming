```cpp run id=fold-seq
#include <iostream>
#include <vector>

using Stack = std::vector<int>;

namespace w {
auto push(int n) {
    return [n](Stack s) { s.push_back(n); return s; };
}

auto lift2(auto f) {
    return [f](Stack s) {
        int b = s.back(); s.pop_back();
        int a = s.back(); s.pop_back();
        s.push_back(f(a, b));
        return s;
    };
}

const auto dup = [](Stack s) { s.push_back(s.back()); return s; };
const auto add = lift2([](int a, int b) { return a + b; });
const auto mul = lift2([](int a, int b) { return a * b; });

// seq(f, g, h) is the program "f g h": apply f, then g, then h.
auto seq(auto... words) {
    return [=](Stack s) { ((s = words(std::move(s))), ...); return s; };
}
}  // namespace w

void show(const Stack& s) {
    for (int x : s) std::cout << x << " ";
    std::cout << "\n";
}

int main() {
    auto square = w::seq(w::dup, w::mul);
    show(w::seq(w::push(2), w::push(3), w::add, square)({}));

    auto left = w::seq(w::seq(w::push(4), w::push(5)), w::seq(w::add, square));
    auto right = w::seq(w::push(4), w::seq(w::push(5), w::add), square);
    show(left({}));
    show(right({}));
}
```

```output for=fold-seq
25 
81 
81 
```

- Verdict: absent. C++ applies functions to arguments. A fold expression over a pack of lambdas composes the words at compile time, so `seq` builds a program that the compiler can inline into straight-line code. Grouping the words in different ways gives the same program, because composition is associative.

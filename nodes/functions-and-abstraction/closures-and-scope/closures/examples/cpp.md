```cpp run id=counter
#include <print>

auto make_counter() {
    return [count = 0]() mutable { return ++count; };
}

int main() {
    auto first = make_counter();
    auto second = make_counter();
    int a = first();
    int b = first();
    int c = first();
    std::println("{} {} {}", a, b, c);
    std::println("{}", second());
    std::println("{} bytes", sizeof first);
}
```

```output for=counter
1 2 3
1
4 bytes
```

- Verdict: native. A lambda expression makes a closure object of a unique, unnamed class type.
- The capture list says how each variable is captured: `[x]` copies, `[&x]` takes a reference, and `[count = 0]` makes a fresh member. A copy is `const` inside the lambda unless the lambda is `mutable`.
- The closure is a value that holds its captures. Here it holds one `int`, so its size is 4 bytes, and it needs no heap allocation.
- The three calls are separate statements because C++ does not specify the order in which the arguments of a function call are evaluated.
- A reference capture does not extend the life of the variable. Returning such a lambda is undefined behaviour (see Pitfalls on the concept page).

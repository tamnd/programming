```cpp run id=continuations
#include <charconv>
#include <functional>
#include <iostream>
#include <string>
#include <string_view>

// std::function gives every continuation one type, so fact can call itself.
long fact(long n, const std::function<long(long)>& k)
{
    if (n == 0) return k(1);
    return fact(n - 1, [&](long r) { return k(n * r); });
}

// Two continuations as template parameters: no allocation, and calls the compiler can inline.
template <class Ok, class Fail>
auto parse(std::string_view s, Ok ok, Fail fail)
{
    int n = 0;
    auto [end, ec] = std::from_chars(s.data(), s.data() + s.size(), n);
    if (ec != std::errc{} || end != s.data() + s.size()) return fail(s);
    return ok(n);
}

int main()
{
    std::cout << fact(5, [](long r) { return r; }) << '\n';
    std::cout << fact(5, [](long r) { return r * 2; }) << '\n';
    for (std::string_view s : {"42", "4x"}) {
        std::cout << parse(
            s, [](int n) { return "ok " + std::to_string(n + 1); },
            [](std::string_view bad) { return "error: not a number: " + std::string(bad); })
                  << '\n';
    }
}
```

```output for=continuations
120
240
ok 43
error: not a number: 4x
```

- Verdict: idiom. Lambdas are continuations, and `parse` takes them as template parameters, so each call site gets its own copy of `parse` with the continuations inlined. The two lambdas must return the same type, because `parse` returns the result of either.
- A recursive CPS function needs one type for all its continuations, as in Rust, so `fact` takes a `std::function`. Each lambda captures `k` and `n` by reference, which is safe only because every continuation runs before `fact` returns. A continuation that is stored and run later must capture by value.

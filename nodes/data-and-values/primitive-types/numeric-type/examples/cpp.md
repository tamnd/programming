```cpp run id=concepts-and-limits
#include <concepts>
#include <limits>
#include <numbers>
#include <numeric>
#include <print>
#include <type_traits>

// A concept limits a template to numeric types.
template <typename T>
    requires std::integral<T> || std::floating_point<T>
T half(T x) { return x / 2; }

int main() {
    // The same template gives integer division for int and real division for double.
    std::println("{} {} {}", half(7), half(7.0), half(-7));

    // short + short has type int, because of integer promotion.
    short s = 1;
    std::println("{} {}", std::is_same_v<decltype(s + s), int>, std::is_same_v<decltype(1 + 1.0f), float>);

    // numeric_limits describes each type.
    std::println("{} {} {} {}", std::numeric_limits<int>::max(), std::numeric_limits<double>::digits,
                 std::numeric_limits<unsigned>::is_signed, std::numeric_limits<float>::has_infinity);

    // std::midpoint does not overflow, unlike (a + b) / 2.
    int a = std::numeric_limits<int>::max() - 2, b = std::numeric_limits<int>::max();
    std::println("{}", std::midpoint(a, b));

    // Mathematical constants are typed.
    std::println("{:.5f} {:.5f}", std::numbers::pi_v<float>, std::numbers::e);
}
```

```output for=concepts-and-limits
3 3.5 -3
true true
2147483647 53 false true
2147483646
3.14159 2.71828
```

- Verdict: native. A template limited by the concepts `std::integral` and `std::floating_point` does integer division for `int` and real division for `double`. `decltype` shows that `short + short` is an `int`. `std::numeric_limits` gives the range of `int` and the 53 bits of a `double`. `std::midpoint` of two large `int` values does not overflow. `<numbers>` gives typed constants such as `pi_v<float>`.

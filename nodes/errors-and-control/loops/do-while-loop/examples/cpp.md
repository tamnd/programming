```cpp run id=to-base-with-a-do-loop
#include <algorithm>
#include <print>
#include <string>

std::string to_base(unsigned n, unsigned base) {
    const char* digits = "0123456789abcdef";
    std::string out;
    do {
        out.push_back(digits[n % base]);
        n /= base;
    } while (n != 0);
    std::ranges::reverse(out);
    return out;
}

int main() {
    for (unsigned n : {0u, 10u, 255u}) {
        std::println("{}: base 2 {}, base 16 {}", n, to_base(n, 2), to_base(n, 16));
    }

    // A post-test loop runs once even if the condition is false.
    int x = 10;
    do {
        std::println("x = {}", x);
        ++x;
    } while (x < 5);
}
```

```output for=to-base-with-a-do-loop
0: base 2 0, base 16 0
10: base 2 1010, base 16 a
255: base 2 11111111, base 16 ff
x = 10
```

- Verdict: native. `to_base` must make at least one digit, so it uses a `do` loop, and 0 gives the string `"0"`. A `while` loop with the same body would give an empty string for 0.

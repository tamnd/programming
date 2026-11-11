```cpp run id=views
#include <iostream>
#include <ranges>
#include <vector>

int main() {
    std::vector<int> readings{18, 22, 19, 25, 30, 12, 40};
    int tested = 0;
    auto hot = readings
        | std::views::filter([&](int t) { ++tested; return t > 20; })
        | std::views::transform([](int t) { return t * 9 / 5 + 32; });

    std::cout << "tested before use: " << tested << "\n";
    for (int f : hot | std::views::take(2)) std::cout << f << " F\n";
    std::cout << "tested: " << tested << "\n";
}
```

```output for=views
tested before use: 0
71 F
77 F
tested: 5
```

- Verdict: idiom. Range adaptors build a lazy, pull-based dataflow pipeline: no element is tested until the loop asks for one, and the loop stops after two results, so the last two readings are never tested. The count is 5 and not 4, because `take` moves the inner iterator one step past the second result before it stops. C++ has no push-based observables in the standard library. RxCpp and Qt's signals and properties give reactive programming in libraries.

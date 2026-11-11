```cpp run id=around
#include <iostream>
#include <string>
#include <utility>

template <typename F>
auto around(std::string name, F f) {
    return [name = std::move(name), f = std::move(f)](auto&&... args) {
        std::cout << "before " << name << "\n";
        auto result = f(std::forward<decltype(args)>(args)...);
        std::cout << "after " << name << " -> " << result << "\n";
        return result;
    };
}

int main() {
    auto area = around("area", [](double w, double h) { return w * h; });
    auto checked = around("checked", [&](double w, double h) {
        if (w < 0 || h < 0) return -1.0;
        return area(w, h);
    });
    std::cout << checked(3, 4) << "\n";
}
```

```output for=around
before checked
before area
after area -> 12
after checked -> 12
12
```

- Verdict: idiom. C++ has no aspects. A generic lambda that wraps another callable gives around advice, and templates let the compiler inline it. AspectC++ is a research weaver that adds AspectJ-style aspects to C++.

```cpp run id=erase
#include <any>
#include <functional>
#include <print>
#include <string>
#include <typeinfo>
#include <vector>

template <typename T>
std::size_t size_of_one(const T&) {
    return sizeof(T);
}

int main() {
    std::println("{} {}", size_of_one('a'), size_of_one(1.0));

    int factor = 3;
    std::vector<std::function<int(int)>> steps;
    steps.push_back([](int x) { return x + 1; });
    steps.push_back([factor](int x) { return x * factor; });
    int value = 2;
    for (const auto& step : steps) {
        value = step(value);
    }
    std::println("{}", value);

    std::any box = std::string("text");
    std::println("{}", box.type() == typeid(std::string));
    try {
        std::println("{}", std::any_cast<int>(box));
    } catch (const std::bad_any_cast&) {
        std::println("bad_any_cast");
    }
}
```

```output for=erase
1 8
9
true
bad_any_cast
```

- Verdict: native. Each use of a template with other template arguments is a separate instantiation, so `size_of_one<char>` and `size_of_one<double>` are two functions.
- `std::function<int(int)>` holds any callable object with that call signature. The two lambdas have different types, but the vector holds them as one type. C++ programmers call this technique type erasure.
- `std::any` keeps a value of any type and remembers its type. `any_cast` with the wrong type throws `std::bad_any_cast`.

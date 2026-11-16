```cpp run id=thunks
#include <functional>
#include <iostream>
#include <optional>
#include <utility>

template <typename T>
class Lazy {
public:
    explicit Lazy(std::function<T()> compute) : compute_(std::move(compute)) {}

    const T& get() {
        if (!value_) {
            value_ = compute_();
            compute_ = nullptr;
        }
        return *value_;
    }

private:
    std::function<T()> compute_;
    std::optional<T> value_;
};

int load() {
    std::cout << "loading\n";
    return 21;
}

int main() {
    auto plain = [] { return load(); };
    std::cout << plain() + plain() << '\n';

    Lazy<int> memo(load);
    std::cout << memo.get() + memo.get() << '\n';
}
```

```output for=thunks
loading
loading
42
loading
42
```

- Verdict: stdlib. A lambda with no parameters, or a `std::function<T()>`, is a plain thunk. `Lazy` stores the result in a `std::optional` and releases the function after the first force.
- A function-local `static` variable is a thread-safe memoized value: it is initialized the first time control passes through its declaration.

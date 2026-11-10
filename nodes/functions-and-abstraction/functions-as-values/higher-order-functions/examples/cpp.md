```cpp run id=hof
#include <algorithm>
#include <functional>
#include <numeric>
#include <print>
#include <string>
#include <vector>

template <typename F>
int twice(F f, int x) {
    return f(f(x));
}

auto compose(auto f, auto g) {
    return [=](auto x) { return f(g(x)); };
}

int main() {
    std::vector<std::string> words{"pear", "fig", "banana"};
    std::println("{}", twice([](int n) { return n * 3; }, 2));
    std::vector<std::size_t> lengths(words.size());
    std::ranges::transform(words, lengths.begin(), &std::string::size);
    std::println("{}", lengths);
    std::vector<int> numbers{1, 2, 3, 4};
    std::println("{}", std::accumulate(numbers.begin(), numbers.end(), 0, std::plus<>{}));
    std::ranges::sort(words, {}, [](const std::string& w) { return w.size(); });
    std::println("{}", words);
    std::function<int(int)> step = compose([](int n) { return n * 2; }, [](int n) { return n + 1; });
    std::println("{}", step(5));
}
```

```output for=hof
18
[4, 3, 6]
10
["fig", "pear", "banana"]
12
```

- Verdict: native. A lambda expression makes a closure object of an unnamed class with a call operator. A function template such as `twice` accepts any callable type.
- The compiler makes a copy of the template for each callable type, so it knows the called function and can inline it.
- `std::function<int(int)>` holds any callable with that signature behind one type. A call of an empty `std::function` throws `std::bad_function_call` (see Pitfalls).
- The ranges algorithms take a projection, here the third argument of `sort`, which plays the role of a key function.

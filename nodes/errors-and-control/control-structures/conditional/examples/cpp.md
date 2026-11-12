```cpp run id=if-constexpr-and-initializer
#include <map>
#include <print>
#include <string>
#include <type_traits>

// if constexpr picks a branch at compile time; the other branch is discarded.
template <typename T>
std::string describe(T value) {
    if constexpr (std::is_integral_v<T>) {
        return "integer " + std::to_string(value * 2);
    } else if constexpr (std::is_floating_point_v<T>) {
        return "floating " + std::to_string(value / 2);
    } else {
        return "text " + std::string(value);
    }
}

int main() {
    std::println("{} | {} | {}", describe(21), describe(3.0), describe("hi"));

    // if with an initializer keeps the iterator inside the statement.
    std::map<std::string, int> stock{{"apple", 3}};
    for (const char* item : {"apple", "pear"}) {
        if (auto it = stock.find(item); it != stock.end()) {
            std::println("{} {}", item, it->second);
        } else {
            std::println("{} none", item);
        }
    }

    // [[likely]] and [[unlikely]] give the compiler a hint about the branch.
    int errors = 0;
    if (errors == 0) [[likely]] {
        std::println("no errors");
    } else [[unlikely]] {
        std::println("{} errors", errors);
    }
}
```

```output for=if-constexpr-and-initializer
integer 42 | floating 1.500000 | text hi
apple 3
pear none
no errors
```

- Verdict: native. `if constexpr` keeps only the branch for the type of the argument, so `describe("hi")` compiles although the integer branch would not work for text. An `if` with an initializer keeps the iterator inside the statement. `[[likely]]` and `[[unlikely]]` are hints to the compiler.

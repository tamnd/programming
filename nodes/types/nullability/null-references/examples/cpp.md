```cpp run id=optional
#include <map>
#include <optional>
#include <print>
#include <string>

std::optional<int> age(const std::map<std::string, int>& ages, const std::string& name) {
    auto it = ages.find(name);
    if (it == ages.end()) {
        return std::nullopt;
    }
    return it->second;
}

int main() {
    std::map<std::string, int> ages{{"ada", 36}};
    for (const char* name : {"ada", "bob"}) {
        if (auto a = age(ages, name)) {
            std::println("{}: {}", name, *a);
        } else {
            std::println("{}: unknown", name);
        }
    }
    std::println("{}", age(ages, "bob").value_or(0));

    int* p = nullptr;
    std::println("{}", p == nullptr);
}
```

```output
ada: 36
bob: unknown
0
true
```

- Verdict: native. A pointer can hold the null pointer value, written `nullptr`, which compares unequal to a pointer to any object. A C++ reference cannot be null in a well-defined program.
- Indirection through a null pointer is undefined behaviour. The language does not check it, but a build with AddressSanitizer reports it at run time (the concept page shows this for C).
- `std::optional<T>` holds a `T` or nothing, and stores the `T` inside the `optional` object with no heap allocation. `value()` throws `std::bad_optional_access` when it is empty. `*` and `->` on an empty `optional` are undefined behaviour.

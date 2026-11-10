```cpp fails expect=sanitizer:address timeout=30 id=string-view
#include <cstddef>
#include <iostream>
#include <string_view>
#include <vector>

// Returns a view of the first claimed bytes of the payload.
std::string_view echo(const std::vector<char>& payload, std::size_t claimed) {
    return std::string_view(payload.data(), claimed);
}

int main() {
    std::vector<char> payload{'h', 'e', 'l', 'l', 'o'};
    std::cout << echo(payload, 64) << '\n';
}
```

The `string_view` constructor takes the length on trust. Printing the view reads 64 bytes from a vector of 5, and AddressSanitizer reports a `heap-buffer-overflow`.

```cpp run flags="-fsanitize=address,undefined" id=span
#include <cstddef>
#include <iostream>
#include <optional>
#include <span>
#include <string_view>
#include <vector>

// Returns the first claimed bytes of the payload, or nothing.
std::optional<std::string_view> echo(std::span<const char> payload, std::size_t claimed) {
    if (claimed > payload.size()) {
        return std::nullopt;
    }
    auto part = payload.first(claimed);
    return std::string_view(part.data(), part.size());
}

int main() {
    std::vector<char> payload{'h', 'e', 'l', 'l', 'o'};
    for (std::size_t claimed : {5, 64}) {
        if (auto reply = echo(payload, claimed)) {
            std::cout << "claimed " << claimed << ": " << *reply << '\n';
        } else {
            std::cout << "claimed " << claimed << ": discarded\n";
        }
    }
}
```

```output for=span
claimed 5: hello
claimed 64: discarded
```

- Verdict: absent. Views and spans carry a size, but their constructors do not check it against the real data. `std::span::first(n)` has the precondition `n <= size()`, and in a build that is not hardened, a violation is undefined behavior.
- Build the view from the container, which knows its size, and check every length from input against `size()` before you narrow the view.
- `std::vector::at` and `std::string::at` check single indices and throw `std::out_of_range`.

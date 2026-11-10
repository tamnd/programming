```cpp run id=ddmin
#include <algorithm>
#include <functional>
#include <print>
#include <string>
#include <vector>

// Returns a 1-minimal part of chars on which fails is still true.
std::string ddmin(std::string chars, const std::function<bool(const std::string &)> &fails) {
    std::size_t n = 2;
    while (chars.size() >= 2) {
        std::size_t size = chars.size();
        std::vector<std::string> subsets, complements;
        for (std::size_t i = 0; i < n; i++) {
            std::size_t lo = i * size / n, hi = (i + 1) * size / n;
            subsets.push_back(chars.substr(lo, hi - lo));
            complements.push_back(chars.substr(0, lo) + chars.substr(hi));
        }
        if (auto s = std::ranges::find_if(subsets, fails); s != subsets.end()) {
            chars = *s; // reduce to subset
            n = 2;
            std::println("subset     {}", chars.size());
            continue;
        }
        if (auto c = std::ranges::find_if(complements, fails); c != complements.end()) {
            chars = *c; // reduce to complement
            n = std::max<std::size_t>(n - 1, 2);
            std::println("complement {}", chars.size());
            continue;
        }
        if (n == size) { // each single character was removed
            break;
        }
        n = std::min(size, 2 * n); // increase granularity
    }
    return chars;
}

int main() {
    int tests = 0;
    // Stands for one run of the browser: it crashes on a <SELECT ...> tag.
    auto browser_crashes = [&tests](const std::string &text) {
        tests++;
        auto start = text.find("<SELECT");
        return start != std::string::npos && text.find('>', start) != std::string::npos;
    };
    const std::string page = R"(<td align=left><SELECT NAME="priority" MULTIPLE SIZE=7>)";
    std::string minimal = ddmin(page, browser_crashes);
    std::println("{} characters reduced to {} in {} tests", page.size(), minimal, tests);
}
```

```output for=ddmin
complement 42
complement 28
complement 21
complement 17
complement 14
complement 13
complement 12
complement 10
complement 9
complement 8
55 characters reduced to <SELECT> in 147 tests
```

- Verdict: idiom. The C++ standard library has no delta debugging.
- `std::ranges::find_if` returns an iterator to the first part that fails, or `end()`.
- A lambda captures the counter by reference, and `std::function` holds it.
- `substr` copies, so each part is a separate string.

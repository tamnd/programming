```cpp run id=precedence
#include <algorithm>
#include <compare>
#include <optional>
#include <print>
#include <ranges>
#include <regex>
#include <string>
#include <vector>

struct Version {
    std::string major, minor, patch;
    std::vector<std::string> pre;
    std::string build;
    std::string text;
};

const std::regex semver(
    R"(^(0|[1-9]\d*)\.(0|[1-9]\d*)\.(0|[1-9]\d*))"
    R"((?:-((?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*)(?:\.(?:0|[1-9]\d*|\d*[a-zA-Z-][0-9a-zA-Z-]*))*))?)"
    R"((?:\+([0-9a-zA-Z-]+(?:\.[0-9a-zA-Z-]+)*))?$)");

std::optional<Version> parse(const std::string& text) {
    std::smatch m;
    if (!std::regex_match(text, m, semver)) return std::nullopt;
    Version v{m[1], m[2], m[3], {}, m[5], text};
    if (m[4].matched) {
        for (auto part : std::views::split(m[4].str(), '.')) v.pre.emplace_back(part.begin(), part.end());
    }
    return v;
}

bool numeric(const std::string& s) {
    return std::ranges::all_of(s, [](char c) { return c >= '0' && c <= '9'; });
}

// Decimal strings with no leading zeros: the longer one is larger.
std::strong_ordering compare_numeric(const std::string& a, const std::string& b) {
    if (auto c = a.size() <=> b.size(); c != 0) return c;
    return a <=> b;
}

std::strong_ordering compare_identifier(const std::string& a, const std::string& b) {
    bool an = numeric(a), bn = numeric(b);
    if (an && bn) return compare_numeric(a, b);
    if (an != bn) return an ? std::strong_ordering::less : std::strong_ordering::greater;
    return a <=> b;
}

std::strong_ordering precedence(const Version& a, const Version& b) {
    if (auto c = compare_numeric(a.major, b.major); c != 0) return c;
    if (auto c = compare_numeric(a.minor, b.minor); c != 0) return c;
    if (auto c = compare_numeric(a.patch, b.patch); c != 0) return c;
    if (a.pre.empty() && b.pre.empty()) return std::strong_ordering::equal;
    if (a.pre.empty()) return std::strong_ordering::greater;
    if (b.pre.empty()) return std::strong_ordering::less;
    for (std::size_t i = 0; i < std::min(a.pre.size(), b.pre.size()); ++i) {
        if (auto c = compare_identifier(a.pre[i], b.pre[i]); c != 0) return c;
    }
    return a.pre.size() <=> b.pre.size();
}

int main() {
    std::vector<std::string> texts{"1.0.0", "1.0.0-rc.1", "1.0.0-beta.11", "1.0.0-beta.2", "1.0.0-beta",
                                   "1.0.0-alpha.beta", "1.0.0-alpha.1", "1.0.0-alpha"};
    std::vector<Version> versions;
    for (const auto& text : texts) versions.push_back(*parse(text));

    std::ranges::sort(versions, [](const Version& a, const Version& b) { return precedence(a, b) < 0; });
    for (const auto& v : versions) std::print("{} ", v.text);
    std::println("");

    std::ranges::sort(texts);
    std::print("string sort:");
    for (const auto& text : texts) std::print(" {}", text);
    std::println("");

    auto a = *parse("1.4.2+build.7");
    auto b = *parse("1.4.2+build.8");
    std::println("same precedence: {}, same text: {}", precedence(a, b) == 0, a.text == b.text);

    for (std::string text : {"v1.2.3", "1.2", "01.2.3", "1.0.0-alpha..1"}) {
        std::println("{}: {}", text, parse(text) ? "valid" : "invalid");
    }
}
```

```output for=precedence
1.0.0-alpha 1.0.0-alpha.1 1.0.0-alpha.beta 1.0.0-beta 1.0.0-beta.2 1.0.0-beta.11 1.0.0-rc.1 1.0.0
string sort: 1.0.0 1.0.0-alpha 1.0.0-alpha.1 1.0.0-alpha.beta 1.0.0-beta 1.0.0-beta.11 1.0.0-beta.2 1.0.0-rc.1
same precedence: true, same text: false
v1.2.3: invalid
1.2: invalid
01.2.3: invalid
1.0.0-alpha..1: invalid
```

- Verdict: idiom. The C++ standard library has no SemVer type. `std::regex` with the default ECMAScript grammar accepts the numbered regex of the specification, and `std::ranges::sort` takes a comparison.
- The example keeps each number as a string and compares the length first, so it has no integer overflow.
- A plain sort of the strings puts 1.0.0 first and beta.11 before beta.2, as the second line of the output shows.

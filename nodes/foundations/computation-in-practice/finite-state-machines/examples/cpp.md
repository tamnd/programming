```cpp run id=constexpr-transition-table
#include <array>
#include <iostream>
#include <regex>
#include <string_view>

enum class S { Start, Sign, Int, Dot, Frac, Dead, Count };
enum class C { Sign, Digit, Dot, Other, Count };

constexpr C kind(char ch) {
    if (ch >= '0' && ch <= '9') return C::Digit;
    if (ch == '+' || ch == '-') return C::Sign;
    return ch == '.' ? C::Dot : C::Other;
}

// one row per state, one column per kind of character
constexpr std::array<std::array<S, 4>, 6> table{{
    {S::Sign, S::Int, S::Dead, S::Dead},  // Start
    {S::Dead, S::Int, S::Dead, S::Dead},  // Sign
    {S::Dead, S::Int, S::Dot, S::Dead},   // Int
    {S::Dead, S::Frac, S::Dead, S::Dead}, // Dot
    {S::Dead, S::Frac, S::Dead, S::Dead}, // Frac
    {S::Dead, S::Dead, S::Dead, S::Dead}, // Dead
}};

constexpr bool accepts(std::string_view text) {
    S s = S::Start;
    for (char ch : text) s = table[static_cast<int>(s)][static_cast<int>(kind(ch))];
    return s == S::Int || s == S::Frac;
}

static_assert(accepts("-3.5") && !accepts("1.")); // runs at compile time

int main() {
    std::regex pattern(R"([+-]?[0-9]+(\.[0-9]+)?)");
    bool agree = true;
    for (std::string_view t : {"42", "-3.5", "1.", "+", "007", ".5", "4.2.1"}) {
        std::cout << t << ' ' << std::boolalpha << accepts(t) << '\n';
        agree = agree && std::regex_match(t.begin(), t.end(), pattern) == accepts(t);
    }
    std::cout << "regex agrees: " << agree << '\n';
}
```

```output for=constexpr-transition-table
42 true
-3.5 true
1. false
+ false
007 true
.5 false
4.2.1 false
regex agrees: true
```

- Verdict: idiom. The C++ file stores the transitions as a table, one row for each state and one column for each kind of character, so a step is one lookup. The table and the function that runs it are `constexpr`, and the `static_assert` runs the machine during compilation. `std::regex` matches the same language.

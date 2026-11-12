```cpp run id=variant-states
#include <iostream>
#include <string>
#include <variant>

struct Idle {};
struct Loading { int percent; };
struct Ready { std::string data; };
struct Failed { std::string error; };

// The state holds exactly one of these, with only the data that the state needs.
using State = std::variant<Idle, Loading, Ready, Failed>;

std::string describe(const State& s) {
    struct Visitor {
        std::string operator()(const Idle&) const { return "idle"; }
        std::string operator()(const Loading& l) const { return "loading " + std::to_string(l.percent) + "%"; }
        std::string operator()(const Ready& r) const { return "ready: " + r.data; }
        std::string operator()(const Failed& f) const { return "failed: " + f.error; }
    };
    return std::visit(Visitor{}, s);
}

int main() {
    State s = Idle{};
    std::cout << describe(s) << '\n';
    s = Loading{40};
    std::cout << describe(s) << '\n';
    s = Ready{"42 rows"};
    std::cout << describe(s) << '\n';
    std::cout << "holds Loading? " << std::boolalpha << std::holds_alternative<Loading>(s) << '\n';
}
```

```output for=variant-states
idle
loading 40%
ready: 42 rows
holds Loading? false
```

- Verdict: native. A class keeps its state in private members and keeps its invariants in its member functions. `std::variant`, from C++17, holds exactly one of a fixed list of types, so each state carries only its own data: a `Loading` state has a percentage and a `Ready` state has the data. `std::visit` calls the overload for the current state, and the compiler rejects a visitor that misses one of the types.

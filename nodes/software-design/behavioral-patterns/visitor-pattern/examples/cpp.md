```cpp run id=expr
#include <iostream>
#include <memory>
#include <string>
#include <variant>

struct Add;
struct Mul;
struct Num {
    int value;
};
using Expr = std::variant<Num, std::unique_ptr<Add>, std::unique_ptr<Mul>>;
struct Add {
    Expr left, right;
};
struct Mul {
    Expr left, right;
};

// overloaded turns a set of lambdas into one callable with an operator() for each type.
template <class... Fs>
struct overloaded : Fs... {
    using Fs::operator()...;
};

std::string show(const Expr& e) {
    return std::visit(
        overloaded{
            [](const Num& n) { return std::to_string(n.value); },
            [](const std::unique_ptr<Add>& a) { return "(" + show(a->left) + " + " + show(a->right) + ")"; },
            [](const std::unique_ptr<Mul>& m) { return "(" + show(m->left) + " * " + show(m->right) + ")"; },
        },
        e);
}

int evaluate(const Expr& e) {
    return std::visit(overloaded{
                          [](const Num& n) { return n.value; },
                          [](const std::unique_ptr<Add>& a) { return evaluate(a->left) + evaluate(a->right); },
                          [](const std::unique_ptr<Mul>& m) { return evaluate(m->left) * evaluate(m->right); },
                      },
                      e);
}

int count(const Expr& e) {
    return std::visit(overloaded{
                          [](const Num&) { return 1; },
                          [](const auto& node) { return 1 + count(node->left) + count(node->right); },
                      },
                      e);
}

Expr num(int v) { return Num{v}; }
Expr add(Expr l, Expr r) { return std::make_unique<Add>(Add{std::move(l), std::move(r)}); }
Expr mul(Expr l, Expr r) { return std::make_unique<Mul>(Mul{std::move(l), std::move(r)}); }

int main() {
    Expr tree = mul(add(num(2), num(3)), num(4));
    std::cout << "show: " << show(tree) << "\n";
    std::cout << "evaluate: " << evaluate(tree) << "\n";
    std::cout << "nodes: " << count(tree) << "\n";
}
```

```output
show: ((2 + 3) * 4)
evaluate: 20
nodes: 5
```

- Verdict: stdlib. `std::variant` is a sum type in the library, and `std::visit` calls the overload that matches the alternative that the variant holds.
- The call to `std::visit` does not compile if one alternative has no matching overload, so a missing case is a compile error, as in Rust.
- `Add` and `Mul` contain `Expr`, so the variant holds them through `std::unique_ptr`. A `Num` needs no pointer.
- In `count`, a generic lambda `const auto&` matches both pointer types. Overload resolution prefers the exact `const Num&` overload for numbers.

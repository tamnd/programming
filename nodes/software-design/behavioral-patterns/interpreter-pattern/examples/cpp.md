```cpp run id=expr
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <variant>

// The grammar as a variant. unique_ptr gives the recursive cases a fixed size.
struct Expr;
using Ptr = std::unique_ptr<Expr>;

struct Num { int value; };
struct Var { std::string name; };
struct Add { Ptr left, right; };
struct Mul { Ptr left, right; };

struct Expr {
    std::variant<Num, Var, Add, Mul> node;
};

Ptr num(int v) { return std::make_unique<Expr>(Expr{Num{v}}); }
Ptr var(std::string n) { return std::make_unique<Expr>(Expr{Var{std::move(n)}}); }
Ptr add(Ptr l, Ptr r) { return std::make_unique<Expr>(Expr{Add{std::move(l), std::move(r)}}); }
Ptr mul(Ptr l, Ptr r) { return std::make_unique<Expr>(Expr{Mul{std::move(l), std::move(r)}}); }

template <class... F> struct overloaded : F... { using F::operator()...; };

using Env = std::map<std::string, int>;

struct unknown_variable : std::runtime_error {
    explicit unknown_variable(const std::string& name)
        : std::runtime_error("unknown variable " + name) {}
};

int evaluate(const Expr& e, const Env& env) {
    return std::visit(overloaded{
        [](const Num& n) { return n.value; },
        [&](const Var& v) {
            auto it = env.find(v.name);
            if (it == env.end()) throw unknown_variable(v.name);
            return it->second;
        },
        [&](const Add& a) { return evaluate(*a.left, env) + evaluate(*a.right, env); },
        [&](const Mul& m) { return evaluate(*m.left, env) * evaluate(*m.right, env); },
    }, e.node);
}

std::string show(const Expr& e);

// A sum inside a product needs parentheses.
std::string wrap(const Expr& e) {
    return std::holds_alternative<Add>(e.node) ? "(" + show(e) + ")" : show(e);
}

std::string show(const Expr& e) {
    return std::visit(overloaded{
        [](const Num& n) { return std::to_string(n.value); },
        [](const Var& v) { return v.name; },
        [](const Add& a) { return show(*a.left) + " + " + show(*a.right); },
        [](const Mul& m) { return wrap(*m.left) + " * " + wrap(*m.right); },
    }, e.node);
}

void report(const Expr& e, const Env& env) {
    std::string label;
    for (const auto& [k, v] : env) label += (label.empty() ? "" : " ") + k + "=" + std::to_string(v);
    try {
        int value = evaluate(e, env);  // before any output, in case it throws
        std::cout << label << ": " << value << "\n";
    } catch (const unknown_variable& err) {
        std::cout << label << ": error: " << err.what() << "\n";
    }
}

int main() {
    Ptr e = mul(var("x"), add(var("y"), num(2)));
    std::cout << "expression: " << show(*e) << "\n";
    report(*e, {{"x", 3}, {"y", 4}});
    report(*e, {{"x", 5}, {"y", 1}});

    Ptr f = add(var("z"), num(1));
    std::cout << "expression: " << show(*f) << "\n";
    report(*f, {{"x", 3}, {"y", 4}});
}
```

```output
expression: x * (y + 2)
x=3 y=4: 18
x=5 y=1: 15
expression: z + 1
x=3 y=4: error: unknown variable z
```


- Verdict: idiom. A `std::variant` holds one node type, and `std::visit` with a set of lambdas gives each operation. The classic form with virtual functions is also valid C++.
- `std::visit` does not compile if a lambda for one of the alternatives is missing, so the compiler checks that each operation covers every node type.
- `Expr` wraps the variant in a struct so that `Add` and `Mul` can hold a `unique_ptr` to a type that is not yet complete. The tree owns its children, and the destructor frees them.
- `overloaded` is the usual helper that joins the lambdas into one function object with an overloaded call operator.

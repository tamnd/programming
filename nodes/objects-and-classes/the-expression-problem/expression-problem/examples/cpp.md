```cpp run id=variant-visit
#include <iostream>
#include <memory>
#include <string>
#include <variant>

// Closed cases, open operations: a std::variant and std::visit.
struct Num;
struct Add;
struct Neg;
using Expr = std::variant<Num, Add, Neg>;

struct Num { int value; };
struct Add { std::shared_ptr<Expr> left, right; };
struct Neg { std::shared_ptr<Expr> inner; };

template <class... Fs>
struct overloaded : Fs... { using Fs::operator()...; };

int eval(const Expr& e)
{
    return std::visit(overloaded{
        [](const Num& n) { return n.value; },
        [](const Add& a) { return eval(*a.left) + eval(*a.right); },
        [](const Neg& g) { return -eval(*g.inner); },
    }, e);
}

// A new operation: one new function. Every arm must be present, or the code does not compile.
std::string show(const Expr& e)
{
    return std::visit(overloaded{
        [](const Num& n) { return std::to_string(n.value); },
        [](const Add& a) { return "(" + show(*a.left) + " + " + show(*a.right) + ")"; },
        [](const Neg& g) { return "-" + show(*g.inner); },
    }, e);
}

std::shared_ptr<Expr> num(int v) { return std::make_shared<Expr>(Num{v}); }
std::shared_ptr<Expr> add(std::shared_ptr<Expr> l, std::shared_ptr<Expr> r)
{
    return std::make_shared<Expr>(Add{l, r});
}
std::shared_ptr<Expr> neg(std::shared_ptr<Expr> x) { return std::make_shared<Expr>(Neg{x}); }

int main()
{
    auto e = add(num(1), neg(add(num(2), num(3))));
    std::cout << show(*e) << " = " << eval(*e) << '\n';
}
```

```output for=variant-visit
(1 + -(2 + 3)) = -4
```

```cpp fails expect=compile id=non-exhaustive-visit
#include <iostream>
#include <variant>

struct Num { int value; };
struct Neg { int inner; };  // a new case
using Expr = std::variant<Num, Neg>;

template <class... Fs>
struct overloaded : Fs... { using Fs::operator()...; };

int eval(const Expr& e)
{
    return std::visit(overloaded{
        [](const Num& n) { return n.value; },
        // no arm for Neg
    }, e);
}

int main()
{
    std::cout << eval(Neg{5}) << '\n';
}
```

- Verdict: idiom. C++ has both forms in the standard language. A class hierarchy with virtual functions keeps the cases open and the operations closed. A `std::variant` with `std::visit` keeps the cases closed and the operations open: a new operation is one new function, and the overload set of lambdas, built by the `overloaded` helper, gives one arm per case. Templates can solve the problem fully, at the price of code that is hard to read.
- The failing block adds `Neg` to the variant without an arm for it. `std::visit` requires a visitor that can take every alternative, so the compiler rejects the call, and every operation that misses the new case fails to compile.

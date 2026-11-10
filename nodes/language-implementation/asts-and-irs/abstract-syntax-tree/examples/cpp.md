```cpp run id=parse-walk
#include <cctype>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

struct Expr;
using Ptr = std::unique_ptr<Expr>;

struct Num { int value; };
struct Var { std::string name; };
struct Bin { char op; Ptr left, right; };

struct Expr {
    std::variant<Num, Var, Bin> node;
};

template <class... F> struct overloaded : F... { using F::operator()...; };

class Parser {
public:
    explicit Parser(std::string src) : src_(std::move(src)) {}

    Ptr expr() {
        Ptr left = term();
        while (peek() == '+' || peek() == '-') {
            char op = src_[pos_++];
            left = std::make_unique<Expr>(Expr{Bin{op, std::move(left), term()}});
        }
        return left;
    }

private:
    char peek() {
        while (pos_ < src_.size() && src_[pos_] == ' ') ++pos_;
        return pos_ < src_.size() ? src_[pos_] : '\0';
    }

    Ptr term() {
        Ptr left = factor();
        while (peek() == '*') {
            ++pos_;
            left = std::make_unique<Expr>(Expr{Bin{'*', std::move(left), factor()}});
        }
        return left;
    }

    Ptr factor() {
        char c = peek();
        if (c == '(') {
            ++pos_;
            Ptr inner = expr();
            if (peek() != ')') throw std::runtime_error("expected ) at " + std::to_string(pos_));
            ++pos_;
            return inner;
        }
        std::size_t start = pos_;
        if (std::isdigit(static_cast<unsigned char>(c))) {
            while (pos_ < src_.size() && std::isdigit(static_cast<unsigned char>(src_[pos_]))) ++pos_;
            return std::make_unique<Expr>(Expr{Num{std::stoi(src_.substr(start, pos_ - start))}});
        }
        if (std::isalpha(static_cast<unsigned char>(c))) {
            while (pos_ < src_.size() && std::isalpha(static_cast<unsigned char>(src_[pos_]))) ++pos_;
            return std::make_unique<Expr>(Expr{Var{src_.substr(start, pos_ - start)}});
        }
        throw std::runtime_error(std::string("unexpected '") + c + "' at " + std::to_string(pos_));
    }

    std::string src_;
    std::size_t pos_ = 0;
};

void show(const Expr& e, int depth = 0) {
    std::string pad(2 * depth, ' ');
    std::visit(overloaded{
        [&](const Num& n) { std::cout << pad << "Num " << n.value << '\n'; },
        [&](const Var& v) { std::cout << pad << "Var " << v.name << '\n'; },
        [&](const Bin& b) {
            std::cout << pad << "Bin " << b.op << '\n';
            show(*b.left, depth + 1);
            show(*b.right, depth + 1);
        },
    }, e.node);
}

int count(const Expr& e) {
    if (auto* b = std::get_if<Bin>(&e.node)) return 1 + count(*b->left) + count(*b->right);
    return 1;
}

void names(const Expr& e, std::vector<std::string>& out) {
    if (auto* v = std::get_if<Var>(&e.node)) out.push_back(v->name);
    if (auto* b = std::get_if<Bin>(&e.node)) {
        names(*b->left, out);
        names(*b->right, out);
    }
}

int eval(const Expr& e, const std::map<std::string, int>& env) {
    return std::visit(overloaded{
        [](const Num& n) { return n.value; },
        [&](const Var& v) { return env.at(v.name); },
        [&](const Bin& b) {
            int l = eval(*b.left, env), r = eval(*b.right, env);
            return b.op == '+' ? l + r : b.op == '-' ? l - r : l * r;
        },
    }, e.node);
}

int main() {
    Ptr tree = Parser("1 + 2 * (3 - x)").expr();
    show(*tree);
    std::cout << "nodes: " << count(*tree) << '\n';
    std::vector<std::string> found;
    names(*tree, found);
    std::cout << "names:";
    for (const auto& n : found) std::cout << ' ' << n;
    std::cout << '\n';
    std::cout << "value at x = 1: " << eval(*tree, {{"x", 1}}) << '\n';
}
```

```output for=parse-walk
Bin +
  Num 1
  Bin *
    Num 2
    Bin -
      Num 3
      Var x
nodes: 7
names: x
value at x = 1: 5
```

- Verdict: idiom. `std::variant` holds one of the node kinds, and `std::visit` with a set of lambdas selects the case. The compiler rejects a visit that misses a kind.
- `Bin` holds its children in `std::unique_ptr`. The type `Expr` is incomplete inside `Bin`, so it cannot hold an `Expr` by value. The tree owns its nodes, and destroying the root frees the whole tree.
- A class hierarchy with a virtual `accept` method is the other common form in C++.

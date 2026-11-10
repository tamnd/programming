```cpp run id=engine
#include <array>
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <set>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

// Values carry their type, as boxed values do in a dynamic language.
using Value = std::variant<long, double>;

std::string kind(const Value& v) { return std::holds_alternative<long>(v) ? "int" : "float"; }

std::string show(const Value& v) {
    std::ostringstream out;
    std::visit([&out](auto x) { out << x; }, v);
    return out.str();
}

// The body of f(x) = x * x + 1 as a tree.
struct Node {
    enum Tag { Num, Arg, Bin } tag;
    long num = 0;
    char op = 0;
    std::shared_ptr<Node> left, right;
};

// Generic arithmetic: check both tags on every operation.
Value arith(char op, const Value& a, const Value& b) {
    return std::visit(
        [op](auto x, auto y) -> Value {
            if (op == '+') return x + y;  // long + long is long; a double makes it double
            return x * y;
        },
        a, b);
}

// The interpreter: walk the tree on every call.
Value interpret(const Node& n, const Value& arg) {
    switch (n.tag) {
    case Node::Num: return n.num;
    case Node::Arg: return arg;
    default: return arith(n.op, interpret(*n.left, arg), interpret(*n.right, arg));
    }
}

// Compiled code for int: closures over raw longs, no tag checks inside.
std::function<long(long)> compile_int(const Node& n) {
    switch (n.tag) {
    case Node::Num: return [v = n.num](long) { return v; };
    case Node::Arg: return [](long x) { return x; };
    default: {
        auto l = compile_int(*n.left), r = compile_int(*n.right);
        if (n.op == '+') return [l, r](long x) { return l(x) + r(x); };
        return [l, r](long x) { return l(x) * r(x); };
    }
    }
}

// Compiled code for any argument: closures that still check tags.
std::function<Value(const Value&)> compile_generic(const Node& n) {
    switch (n.tag) {
    case Node::Num: return [v = Value(n.num)](const Value&) { return v; };
    case Node::Arg: return [](const Value& a) { return a; };
    default: {
        auto l = compile_generic(*n.left), r = compile_generic(*n.right);
        return [op = n.op, l, r](const Value& a) { return arith(op, l(a), r(a)); };
    }
    }
}

constexpr int THRESHOLD = 3;

// Compiled code; it returns std::nullopt when the guard fails.
using Code = std::function<std::optional<Value>(const Value&)>;

class JitFunction {
public:
    JitFunction(std::string name, Node body) : name_(std::move(name)), body_(std::move(body)) {}

    std::pair<Value, std::string> call(const Value& arg) {
        if (!code_ && calls_ >= THRESHOLD) compile();
        if (code_) {
            if (auto result = code_(arg)) {
                counts_[1]++;
                return {*result, "compiled, " + code_kind_};
            }
            std::cout << "* deoptimize " << name_ << ": guard failed, " << show(arg) << " is not an int\n";
            counts_[2]++;
            code_ = nullptr;
            calls_ = 0;
        }
        calls_++;
        counts_[0]++;
        seen_.insert(kind(arg));
        return {interpret(body_, arg), "interpreter, call " + std::to_string(calls_)};
    }

    const std::array<int, 3>& counts() const { return counts_; }

private:
    void compile() {
        std::string how;
        if (seen_ == std::set<std::string>{"int"}) {
            code_ = [raw = compile_int(body_)](const Value& a) -> std::optional<Value> {
                if (auto n = std::get_if<long>(&a)) return Value(raw(*n));  // the guard
                return std::nullopt;
            };
            code_kind_ = "int";
            how = "specialized for int";
        } else {
            code_ = [generic = compile_generic(body_)](const Value& a) -> std::optional<Value> { return generic(a); };
            code_kind_ = how = "generic";
        }
        std::string kinds;
        for (const auto& k : seen_) kinds += (kinds.empty() ? "" : ", ") + k;
        std::cout << "* compile " << name_ << ": arguments seen: " << kinds << "; " << how << "\n";
    }

    std::string name_;
    Node body_;
    int calls_ = 0;               // calls in the interpreter since the last compilation
    std::set<std::string> seen_;  // the profile: the kinds of the arguments
    Code code_;                   // compiled code, or empty
    std::string code_kind_;
    std::array<int, 3> counts_{};  // interpreter, compiled, deoptimizations
};

int main() {
    auto x = std::make_shared<Node>(Node{.tag = Node::Arg});
    auto one = std::make_shared<Node>(Node{.tag = Node::Num, .num = 1});
    auto square = std::make_shared<Node>(Node{Node::Bin, 0, '*', x, x});
    JitFunction f("f", Node{Node::Bin, 0, '+', square, one});

    std::vector<Value> inputs{1L, 2L, 3L, 4L, 2.5, 6L, 7L, 8L};
    for (const auto& arg : inputs) {
        auto [result, tier] = f.call(arg);
        std::cout << "f(" << show(arg) << ") = " << show(result) << " [" << tier << "]\n";
    }
    auto [i, c, d] = f.counts();
    std::cout << "interpreter: " << i << ", compiled: " << c << ", deoptimizations: " << d << "\n";
}
```

```output for=engine
f(1) = 2 [interpreter, call 1]
f(2) = 5 [interpreter, call 2]
f(3) = 10 [interpreter, call 3]
* compile f: arguments seen: int; specialized for int
f(4) = 17 [compiled, int]
* deoptimize f: guard failed, 2.5 is not an int
f(2.5) = 7.25 [interpreter, call 1]
f(6) = 37 [interpreter, call 2]
f(7) = 50 [interpreter, call 3]
* compile f: arguments seen: float, int; generic
f(8) = 65 [compiled, generic]
interpreter: 6, compiled: 2, deoptimizations: 1
```

- Verdict: idiom. C++ compiles ahead of time to machine code, so a JIT is something a program builds, as here.
- `Value` is a `std::variant<long, double>`, a sum type from the standard library. `std::get_if<long>` is the guard: it returns a null pointer when the value holds a `double`.
- `arith` calls `std::visit` with a generic lambda. The compiler makes one copy of the lambda for each pair of types, and the usual arithmetic conversions make `long * double` a `double`.
- Compiled code is a `std::function` that holds a lambda. Each lambda captures the code of its children by value, so the tree of closures owns itself.

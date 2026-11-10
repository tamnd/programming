```cpp run id=interpreter
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

struct Num;
struct Var;
struct Bin;
using Expr = std::variant<Num, Var, Bin>;

struct Num { long value; };
struct Var { std::string name; };
struct Bin {
    char op;
    std::unique_ptr<Expr> left, right;
};

struct Let;
struct Assign;
struct Block;
struct While;
struct Print;
using Stmt = std::variant<Let, Assign, Block, While, Print>;

struct Let { std::string name; Expr init; };
struct Assign { std::string name; Expr value; };
struct Block { std::vector<Stmt> body; };
struct While { Expr cond; std::unique_ptr<Stmt> body; };
struct Print { Expr value; };

struct Env {
    std::unordered_map<std::string, long> vars;
    Env* parent;
};

struct Interpreter {
    int nodes = 0, lookups = 0, links = 0;

    Env& find(Env& env, const std::string& name) {
        ++lookups;
        for (Env* e = &env; e != nullptr; e = e->parent) {
            if (e->vars.count(name)) return *e;
            ++links;
        }
        throw std::runtime_error("undefined variable '" + name + "'");
    }

    long evaluate(const Expr& node, Env& env) {
        ++nodes;
        if (auto* n = std::get_if<Num>(&node)) return n->value;
        if (auto* v = std::get_if<Var>(&node)) return find(env, v->name).vars[v->name];
        const Bin& b = std::get<Bin>(node);
        long x = evaluate(*b.left, env);
        long y = evaluate(*b.right, env);
        switch (b.op) {
            case '+': return x + y;
            case '-': return x - y;
            case '*': return x * y;
            case '<': return x < y ? 1 : 0;
        }
        throw std::runtime_error(std::string("unknown operator ") + b.op);
    }

    void execute(const Stmt& node, Env& env) {
        ++nodes;
        std::visit([&](const auto& s) { run(s, env); }, node);
    }

    void run(const Let& s, Env& env) { env.vars[s.name] = evaluate(s.init, env); }

    void run(const Assign& s, Env& env) {
        long result = evaluate(s.value, env);
        find(env, s.name).vars[s.name] = result;
    }

    void run(const Block& s, Env& env) {
        Env inner{{}, &env};  // lives on the C++ stack until the block ends
        for (const Stmt& stmt : s.body) execute(stmt, inner);
    }

    void run(const While& s, Env& env) {
        while (evaluate(s.cond, env) != 0) execute(*s.body, env);
    }

    void run(const Print& s, Env& env) { std::cout << evaluate(s.value, env) << '\n'; }
};

std::unique_ptr<Expr> box(Expr e) { return std::make_unique<Expr>(std::move(e)); }
Expr bin(char op, Expr l, Expr r) { return Bin{op, box(std::move(l)), box(std::move(r))}; }

int main() {
    std::vector<Stmt> loop_body;
    loop_body.push_back(Let{"step", Var{"n"}});
    loop_body.push_back(Assign{"acc", bin('*', Var{"acc"}, Var{"step"})});
    loop_body.push_back(Assign{"n", bin('-', Var{"n"}, Num{1})});

    std::vector<Stmt> body;
    body.push_back(Let{"n", Num{5}});
    body.push_back(Let{"acc", Num{1}});
    body.push_back(While{bin('<', Num{0}, Var{"n"}), std::make_unique<Stmt>(Block{std::move(loop_body)})});
    body.push_back(Print{Var{"acc"}});
    Stmt program = Block{std::move(body)};

    Interpreter interp;
    Env global{{}, nullptr};
    interp.execute(program, global);
    std::cout << "nodes visited: " << interp.nodes << '\n';
    std::cout << "variable lookups: " << interp.lookups << ", parent links followed: " << interp.links << '\n';
}
```

```output for=interpreter
120
nodes visited: 81
variable lookups: 37, parent links followed: 25
```

- Verdict: idiom. `std::variant` holds one of the node types, and `std::unique_ptr` breaks the recursion of a type that contains itself. `evaluate` uses `std::get_if`, and `execute` uses `std::visit` with one overload of `run` for each statement type.
- The environment of a block is a local variable of `run(const Block&, ...)`. It dies when the block ends, and no garbage collector is needed. This works only because nothing can keep a pointer to it after the block ends. With closures, an environment can outlive its block and must move to the heap.
- The children are `unique_ptr`, so nodes cannot be copied. The program is built with `push_back` and `std::move`, because an initializer list copies its elements.

```java run id=interpreter
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class Main {
    sealed interface Expr permits Num, Var, Bin {}
    record Num(long value) implements Expr {}
    record Var(String name) implements Expr {}
    record Bin(char op, Expr left, Expr right) implements Expr {}

    sealed interface Stmt permits Let, Assign, Block, While, Print {}
    record Let(String name, Expr init) implements Stmt {}
    record Assign(String name, Expr value) implements Stmt {}
    record Block(List<Stmt> body) implements Stmt {}
    record While(Expr cond, Stmt body) implements Stmt {}
    record Print(Expr value) implements Stmt {}

    static final class Env {
        final Map<String, Long> vars = new HashMap<>();
        final Env parent;

        Env(Env parent) { this.parent = parent; }
    }

    static final class Interpreter {
        int nodes, lookups, links;

        Env find(Env env, String name) {
            lookups++;
            for (Env e = env; e != null; e = e.parent) {
                if (e.vars.containsKey(name)) return e;
                links++;
            }
            throw new IllegalStateException("undefined variable '" + name + "'");
        }

        long evaluate(Expr node, Env env) {
            nodes++;
            return switch (node) {
                case Num(long value) -> value;
                case Var(String name) -> find(env, name).vars.get(name);
                case Bin(char op, Expr left, Expr right) -> {
                    long a = evaluate(left, env);
                    long b = evaluate(right, env);
                    yield switch (op) {
                        case '+' -> a + b;
                        case '-' -> a - b;
                        case '*' -> a * b;
                        case '<' -> a < b ? 1 : 0;
                        default -> throw new IllegalStateException("unknown operator " + op);
                    };
                }
            };
        }

        void execute(Stmt node, Env env) {
            nodes++;
            switch (node) {
                case Let(String name, Expr init) -> env.vars.put(name, evaluate(init, env));
                case Assign(String name, Expr value) -> {
                    long result = evaluate(value, env);
                    find(env, name).vars.put(name, result);
                }
                case Block(List<Stmt> body) -> {
                    Env inner = new Env(env);
                    for (Stmt stmt : body) execute(stmt, inner);
                }
                case While(Expr cond, Stmt body) -> {
                    while (evaluate(cond, env) != 0) execute(body, env);
                }
                case Print(Expr value) -> System.out.println(evaluate(value, env));
            }
        }
    }

    public static void main(String[] args) {
        Stmt program = new Block(List.of(
            new Let("n", new Num(5)),
            new Let("acc", new Num(1)),
            new While(new Bin('<', new Num(0), new Var("n")), new Block(List.of(
                new Let("step", new Var("n")),
                new Assign("acc", new Bin('*', new Var("acc"), new Var("step"))),
                new Assign("n", new Bin('-', new Var("n"), new Num(1)))))),
            new Print(new Var("acc"))));

        Interpreter interp = new Interpreter();
        interp.execute(program, new Env(null));
        System.out.println("nodes visited: " + interp.nodes);
        System.out.println("variable lookups: " + interp.lookups + ", parent links followed: " + interp.links);
    }
}
```

```output for=interpreter
120
nodes visited: 81
variable lookups: 37, parent links followed: 25
```

- Verdict: idiom. Sealed interfaces and records model the tree, and `switch` with record patterns takes each node apart.
- The `switch` over a sealed interface needs no `default`. The compiler knows every permitted record, and it rejects the switch if a case is missing.
- Crafting Interpreters writes its Java interpreter, jlox, with the Visitor pattern instead. Record patterns put the same dispatch in one method, without an `accept` method on each class.

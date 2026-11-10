```java run id=expr
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.stream.Collectors;

public class Main {
    // The grammar: a sealed interface and one record for each rule.
    sealed interface Expr permits Num, Var, Add, Mul {}
    record Num(int value) implements Expr {}
    record Var(String name) implements Expr {}
    record Add(Expr left, Expr right) implements Expr {}
    record Mul(Expr left, Expr right) implements Expr {}

    static class UnknownVariable extends RuntimeException {
        UnknownVariable(String name) { super("unknown variable " + name); }
    }

    static int evaluate(Expr e, Map<String, Integer> env) {
        return switch (e) {
            case Num n -> n.value();
            case Var v -> {
                Integer value = env.get(v.name());
                if (value == null) throw new UnknownVariable(v.name());
                yield value;
            }
            case Add(Expr l, Expr r) -> evaluate(l, env) + evaluate(r, env);
            case Mul(Expr l, Expr r) -> evaluate(l, env) * evaluate(r, env);
        };
    }

    static String show(Expr e) {
        return switch (e) {
            case Num n -> Integer.toString(n.value());
            case Var v -> v.name();
            case Add(Expr l, Expr r) -> show(l) + " + " + show(r);
            case Mul(Expr l, Expr r) -> wrap(l) + " * " + wrap(r);
        };
    }

    // A sum inside a product needs parentheses.
    static String wrap(Expr e) {
        return e instanceof Add ? "(" + show(e) + ")" : show(e);
    }

    static void report(Expr e, Map<String, Integer> env) {
        String label = env.entrySet().stream()
                .map(kv -> kv.getKey() + "=" + kv.getValue())
                .collect(Collectors.joining(" "));
        try {
            System.out.println(label + ": " + evaluate(e, env));
        } catch (UnknownVariable err) {
            System.out.println(label + ": error: " + err.getMessage());
        }
    }

    static Map<String, Integer> env(String k1, int v1, String k2, int v2) {
        Map<String, Integer> m = new LinkedHashMap<>();
        m.put(k1, v1);
        m.put(k2, v2);
        return m;
    }

    public static void main(String[] args) {
        Expr e = new Mul(new Var("x"), new Add(new Var("y"), new Num(2)));
        System.out.println("expression: " + show(e));
        report(e, env("x", 3, "y", 4));
        report(e, env("x", 5, "y", 1));

        Expr f = new Add(new Var("z"), new Num(1));
        System.out.println("expression: " + show(f));
        report(f, env("x", 3, "y", 4));
    }
}
```

```output
expression: x * (y + 2)
x=3 y=4: 18
x=5 y=1: 15
expression: z + 1
x=3 y=4: error: unknown variable z
```

- Verdict: idiom. Records give the nodes, a sealed interface closes the set of node types, and a `switch` with record patterns gives each operation.
- The `switch` has no `default`. The compiler accepts it because the interface is sealed and every permitted type has a `case`. A fifth record in the `permits` list makes both switches fail to compile.
- `case Add(Expr l, Expr r)` takes the record apart in the pattern, so the code names the children without calls to the accessors.
- The classic form, with an abstract `evaluate` method in each class, is also valid Java. The C# example shows it.

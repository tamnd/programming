```java run id=engine
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.TreeSet;
import java.util.function.LongBinaryOperator;
import java.util.function.LongUnaryOperator;
import java.util.function.UnaryOperator;

public class Main {
    // Values carry their type, as boxed values do in a dynamic language.
    sealed interface Value permits IntV, FloatV {}
    record IntV(long n) implements Value { public String toString() { return Long.toString(n); } }
    record FloatV(double x) implements Value { public String toString() { return Double.toString(x); } }

    // The body of f(x) = x * x + 1 as a tree.
    sealed interface Node permits Num, Arg, Bin {}
    record Num(long value) implements Node {}
    record Arg() implements Node {}
    record Bin(char op, Node left, Node right) implements Node {}

    static LongBinaryOperator intOp(char op) { return op == '+' ? Long::sum : (a, b) -> a * b; }

    static double asDouble(Value v) {
        return switch (v) {
            case IntV i -> i.n();
            case FloatV f -> f.x();
        };
    }

    // Generic arithmetic: check both tags on every operation.
    static Value arith(char op, Value a, Value b) {
        if (a instanceof IntV(long x) && b instanceof IntV(long y)) return new IntV(intOp(op).applyAsLong(x, y));
        double x = asDouble(a), y = asDouble(b);
        return new FloatV(op == '+' ? x + y : x * y);
    }

    // The interpreter: walk the tree on every call.
    static Value interpret(Node node, Value arg) {
        return switch (node) {
            case Num n -> new IntV(n.value());
            case Arg a -> arg;
            case Bin(char op, Node l, Node r) -> arith(op, interpret(l, arg), interpret(r, arg));
        };
    }

    // Compiled code for int: closures over raw longs, no tag checks inside.
    static LongUnaryOperator compileInt(Node node) {
        return switch (node) {
            case Num n -> { long v = n.value(); yield x -> v; }
            case Arg a -> x -> x;
            case Bin(char op, Node l, Node r) -> {
                LongBinaryOperator f = intOp(op);
                LongUnaryOperator cl = compileInt(l), cr = compileInt(r);
                yield x -> f.applyAsLong(cl.applyAsLong(x), cr.applyAsLong(x));
            }
        };
    }

    // Compiled code for any argument: closures that still check tags.
    static UnaryOperator<Value> compileGeneric(Node node) {
        return switch (node) {
            case Num n -> { Value boxed = new IntV(n.value()); yield a -> boxed; }
            case Arg a -> x -> x;
            case Bin(char op, Node l, Node r) -> {
                UnaryOperator<Value> cl = compileGeneric(l), cr = compileGeneric(r);
                yield a -> arith(op, cl.apply(a), cr.apply(a));
            }
        };
    }

    static final int THRESHOLD = 3;

    static final class JitFunction {
        final String name;
        final Node body;
        int calls = 0; // calls in the interpreter since the last compilation
        final TreeSet<String> seen = new TreeSet<>(); // the profile: the kinds of the arguments
        UnaryOperator<Value> code = null; // compiled code; it returns null when the guard fails
        String kind = "";
        final Map<String, Integer> counts = new LinkedHashMap<>();

        JitFunction(String name, Node body) {
            this.name = name;
            this.body = body;
            for (String k : List.of("interpreter", "compiled", "deoptimizations")) counts.put(k, 0);
        }

        void compile() {
            String how;
            if (seen.equals(new TreeSet<>(List.of("int")))) {
                LongUnaryOperator raw = compileInt(body);
                code = a -> a instanceof IntV(long n) ? new IntV(raw.applyAsLong(n)) : null; // the guard
                kind = "int";
                how = "specialized for int";
            } else {
                code = compileGeneric(body);
                kind = how = "generic";
            }
            System.out.println("* compile " + name + ": arguments seen: " + String.join(", ", seen) + "; " + how);
        }

        String[] call(Value arg) {
            if (code == null && calls >= THRESHOLD) compile();
            if (code != null) {
                Value result = code.apply(arg);
                if (result != null) {
                    counts.merge("compiled", 1, Integer::sum);
                    return new String[] {result.toString(), "compiled, " + kind};
                }
                System.out.println("* deoptimize " + name + ": guard failed, " + arg + " is not an int");
                counts.merge("deoptimizations", 1, Integer::sum);
                code = null;
                calls = 0;
            }
            calls++;
            counts.merge("interpreter", 1, Integer::sum);
            seen.add(arg instanceof IntV ? "int" : "float");
            return new String[] {interpret(body, arg).toString(), "interpreter, call " + calls};
        }
    }

    public static void main(String[] args) {
        JitFunction f = new JitFunction("f", new Bin('+', new Bin('*', new Arg(), new Arg()), new Num(1)));
        List<Value> inputs = List.of(new IntV(1), new IntV(2), new IntV(3), new IntV(4),
                new FloatV(2.5), new IntV(6), new IntV(7), new IntV(8));
        for (Value arg : inputs) {
            String[] r = f.call(arg);
            System.out.println("f(" + arg + ") = " + r[0] + " [" + r[1] + "]");
        }
        List<String> parts = new ArrayList<>();
        f.counts.forEach((k, v) -> parts.add(k + ": " + v));
        System.out.println(String.join(", ", parts));
    }
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

The second program asks the running JVM about its own compilation system. `getCompilationMXBean` returns `null` when the JVM has no compilation system, for example when it runs with `-Xint`.

```java run id=hotspot
import java.lang.management.CompilationMXBean;
import java.lang.management.ManagementFactory;

public class Main {
    public static void main(String[] args) {
        CompilationMXBean jit = ManagementFactory.getCompilationMXBean();
        System.out.println("compiler: " + (jit == null ? "none" : jit.getName()));
        System.out.println("mode: " + System.getProperty("java.vm.info"));
    }
}
```

```output for=hotspot
compiler: HotSpot 64-Bit Tiered Compilers
mode: mixed mode, sharing
```

- Verdict: native. HotSpot interprets bytecode and compiles hot methods to native code. This mixed mode is the default, and `-Xint` turns compilation off.
- The name that HotSpot reports for its compilation system says that the compilers are tiered. Tiered compilation is on by default in the server VM.
- Sealed interfaces and record patterns model the values and the tree. `a instanceof IntV(long n)` is the guard: it tests the tag and unboxes the value in one step.
- `LongUnaryOperator` works on primitive `long` values, so the compiled code for `int` does not box its intermediate results.

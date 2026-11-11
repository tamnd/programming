```java run id=continuations
import java.util.List;
import java.util.function.Function;
import java.util.function.IntFunction;
import java.util.function.Supplier;

public class Main {
    // CPS: the result goes to k, and k decides what happens next
    static <R> R fact(int n, Function<Integer, R> k) {
        if (n == 0) return k.apply(1);
        return fact(n - 1, r -> k.apply(n * r));
    }

    // two continuations: one for a match, one for no match
    static <R> R find(List<String> xs, String prefix, IntFunction<R> found, Supplier<R> missing) {
        for (int i = 0; i < xs.size(); i++)
            if (xs.get(i).startsWith(prefix)) return found.apply(i);
        return missing.get();
    }

    public static void main(String[] args) {
        int n = fact(5, r -> r);
        String s = fact(5, r -> "5! = " + r);
        System.out.println(n + " " + s);
        List<String> files = List.of("a.txt", "b.png", "c.txt");
        System.out.println(find(files, "b", i -> "found at " + i, () -> "not found"));
        System.out.println(find(files, "z", i -> "found at " + i, () -> "not found"));
        try {
            fact(100_000, r -> r);
        } catch (StackOverflowError e) {
            System.out.println("StackOverflowError");
        }
    }
}
```

```output for=continuations
120 5! = 120
found at 1
not found
StackOverflowError
```

- Verdict: idiom. A continuation in Java is an object of a functional interface such as `Function`, `IntFunction` or `Supplier`, and a lambda creates one. `fact` is generic in the result type `R`, so the same function returns a number to one caller and a string to another. `find` takes two continuations and calls exactly one of them, which replaces a return value that could be absent.
- The JVM does not eliminate tail calls, so each step of `fact` adds a stack frame, and 100,000 steps throw `StackOverflowError`. `CompletableFuture.thenApply` and `thenCompose` are the standard library form of CPS for asynchronous work: each takes the continuation for the result of a future.

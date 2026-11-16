```java run id=void-and-Void
import java.util.concurrent.CompletableFuture;
import java.util.function.Supplier;

public class Main {
  static void greet(String name) {
    System.out.println("hello " + name);
  }

  // A generic method needs a type argument for "no result", so Void stands in.
  static <T> T timed(String label, Supplier<T> body) {
    T result = body.get();
    System.out.println(label + " done");
    return result;
  }

  public static void main(String[] args) {
    Integer n = timed("sum", () -> 1 + 2);
    System.out.println(n);
    Void v = timed("greet", () -> {
      greet("Ann");
      return null;
    });
    System.out.println(v);

    // Runnable has a void method, and Supplier has a result.
    Runnable r = () -> greet("Bo");
    r.run();

    CompletableFuture<Void> f = CompletableFuture.runAsync(() -> {});
    System.out.println(f.join());

    System.out.println(void.class + " " + Void.class.getName() + " " + Void.TYPE);
  }
}
```

```output for=void-and-Void
sum done
3
hello Ann
greet done
null
hello Bo
null
void java.lang.Void void
```

- Verdict: stdlib. `void` marks a method without a result, but `void` cannot be a type argument, so `Supplier<void>` does not compile. Code uses `Void` instead, and because `Void` has no instances, the only value it can hold is `null`. That is why the lambda for `greet` ends with `return null`, and why `CompletableFuture<Void>` gives `null`. `Runnable` and `Supplier` are two interfaces for the cases with and without a result. `Void.TYPE` is the `Class` object for the keyword `void`.

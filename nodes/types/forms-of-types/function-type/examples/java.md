```java run id=functional-interfaces
import java.util.function.BiFunction;
import java.util.function.Function;
import java.util.function.IntUnaryOperator;
import java.util.function.Supplier;

public class Main {
  static int twice(IntUnaryOperator f, int x) {
    return f.applyAsInt(f.applyAsInt(x));
  }

  public static void main(String[] args) {
    Function<Integer, Integer> inc = n -> n + 1;
    IntUnaryOperator incInt = n -> n + 1;
    System.out.println(inc.apply(5) + " " + twice(incInt, 5));

    Function<Integer, Integer> times10 = n -> n * 10;
    Function<Integer, Integer> both = inc.compose(times10);
    System.out.println(both.apply(4));

    BiFunction<String, Integer, String> repeat = String::repeat;
    Supplier<String> hello = () -> "hello";
    System.out.println(repeat.apply("ab", 3) + " " + hello.get());

    // A Function is not an IntUnaryOperator, so a method reference converts it.
    IntUnaryOperator fromInc = inc::apply;
    System.out.println(twice(fromInc, 1));
  }
}
```

```output for=functional-interfaces
6 7
41
ababab hello
3
```

- Verdict: native. Java has no arrow type. A lambda or a method reference takes the type of the functional interface that the context expects, such as `Function<Integer, Integer>`, `IntUnaryOperator`, `BiFunction` or `Supplier`. `inc.compose(times10)` runs `times10` first, so the result for 4 is 41. `inc` cannot go to `twice` directly, because its type is `Function`, but the method reference `inc::apply` makes an `IntUnaryOperator` from it.

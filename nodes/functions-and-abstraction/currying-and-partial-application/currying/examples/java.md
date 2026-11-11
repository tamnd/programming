```java run id=functions
import java.util.function.BiFunction;
import java.util.function.Function;

public class Main {
    static <A, B, R> Function<B, R> partial(BiFunction<A, B, R> f, A a) {
        return b -> f.apply(a, b);
    }

    public static void main(String[] args) {
        Function<Integer, Function<Integer, Integer>> add = a -> b -> a + b;
        Function<Integer, Integer> addTen = add.apply(10);
        System.out.println(addTen.apply(5));

        BiFunction<String, Integer, String> repeat = String::repeat;
        Function<Integer, String> stars = partial(repeat, "*");
        System.out.println(stars.apply(3));
    }
}
```

```output for=functions
15
***
```

- Verdict: idiom. A lambda can return a lambda, so `Function<A, Function<B, R>>` is a curried function. The types are long, and each call is `apply`.
- Java has no `partial` in the standard library. A lambda that captures the fixed arguments does the same job. The captured local variables must be effectively final.

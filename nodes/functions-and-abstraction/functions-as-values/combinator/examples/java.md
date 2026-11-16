```java run id=combinator-java
import java.util.function.BiFunction;
import java.util.function.Function;
import java.util.function.Predicate;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

public class Main {
    static <A, B, C> BiFunction<B, A, C> flip(BiFunction<A, B, C> f) {
        return (b, a) -> f.apply(a, b);
    }

    static <A, B> Function<B, A> constant(A x) {
        return ignored -> x;
    }

    public static void main(String[] args) {
        Function<Integer, Integer> id = Function.identity();
        System.out.println(id.apply(5));
        System.out.println(Main.<Integer, Integer>constant(7).apply(99));
        BiFunction<Integer, Integer, Integer> sub = (a, b) -> a - b;
        System.out.println(flip(sub).apply(2, 10));
        Predicate<Integer> isEven = x -> x % 2 == 0;
        Predicate<Integer> keep = isEven.and(x -> x > 0);
        System.out.println(IntStream.rangeClosed(-2, 4).boxed()
                .filter(keep)
                .map(String::valueOf)
                .collect(Collectors.joining(" ")));
    }
}
```

```output for=combinator-java
5
7
8
2 4
```

- Verdict: stdlib. `Function.identity` and `Predicate.and` are in the library. A generic method that returns a lambda gives `flip` and `constant`.

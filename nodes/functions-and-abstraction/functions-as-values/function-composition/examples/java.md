```java run id=function-composition-java
import java.util.List;
import java.util.function.Function;

public class Main {
    public static void main(String[] args) {
        Function<Integer, Integer> inc = x -> x + 1;
        Function<Integer, Integer> twice = x -> x * 2;

        System.out.println(twice.compose(inc).apply(5));
        System.out.println(inc.andThen(twice).apply(5));
        System.out.println(inc.compose(twice).apply(5));

        Function<Integer, Integer> all = List.of(inc, twice, inc).stream()
                .reduce(Function.identity(), Function::andThen);
        System.out.println(all.apply(5));
    }
}
```

```output for=function-composition-java
12
12
11
13
```

- Verdict: stdlib. `Function.compose` runs its argument first, and `Function.andThen` runs its argument last. `Function.identity` is the start value when a stream reduces a list of functions to one.

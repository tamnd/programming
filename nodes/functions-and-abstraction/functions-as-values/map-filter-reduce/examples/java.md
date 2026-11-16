```java run id=map-filter-reduce-java
import java.util.List;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

public class Main {
    public static void main(String[] args) {
        List<Integer> squares = IntStream.rangeClosed(1, 6)
                .filter(x -> x % 2 == 0)
                .map(x -> x * x)
                .boxed()
                .toList();
        System.out.println(squares.stream()
                .map(String::valueOf)
                .collect(Collectors.joining(" ")));
        System.out.println(squares.stream().reduce(0, Integer::sum));
    }
}
```

```output for=map-filter-reduce-java
4 16 36
56
```

- Verdict: stdlib. The package `java.util.stream` has `filter`, `map` and `reduce`. The intermediate operations are lazy, and the terminal operation `toList` or `reduce` runs the pipeline.

```java run id=streams
import java.util.List;
import java.util.function.Function;
import java.util.stream.Collectors;

public class Main {
    record Order(String item, int qty, int price) {}

    public static void main(String[] args) {
        List<Order> orders = List.of(new Order("pen", 3, 2), new Order("book", 1, 12), new Order("ink", 2, 5));

        int total = orders.stream()
                .filter(o -> o.qty() > 1)
                .mapToInt(o -> o.qty() * o.price())
                .sum();
        System.out.println(total);

        Function<String, String> trim = String::strip;
        Function<String, String> slug = trim.andThen(String::toLowerCase).andThen(s -> s.replace(' ', '-'));
        System.out.println(slug.apply("  Pure Functions  "));

        System.out.println(orders.stream()
                .collect(Collectors.groupingBy(o -> o.qty() > 1, Collectors.counting())));
    }
}
```

```output for=streams
16
pure-functions
{false=1, true=2}
```

- Verdict: native. Java has lambdas, method references, the interfaces of `java.util.function` and streams. The `java.util.stream` package describes itself as "Classes to support functional-style operations on streams of elements".
- `List.of` and records give immutable values. The stream documentation warns that results "may be nondeterministic or incorrect" when a lambda in a pipeline is stateful.

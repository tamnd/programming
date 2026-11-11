```java run id=collectors
import java.util.List;
import java.util.Map;
import java.util.TreeMap;
import java.util.regex.Pattern;
import java.util.stream.Collectors;

public class Main {
    record Order(String item, int qty, int price) {}

    public static void main(String[] args) {
        List<Order> orders = List.of(new Order("pen", 3, 2), new Order("book", 1, 12), new Order("ink", 2, 5), new Order("pen", 4, 2));

        Map<String, Integer> totals = orders.stream()
                .collect(Collectors.groupingBy(Order::item, TreeMap::new, Collectors.summingInt(o -> o.qty() * o.price())));
        System.out.println(totals);

        var m = Pattern.compile("\\b(\\d{4})-(\\d{2})-(\\d{2})\\b").matcher("from 2024-01-31 to 2025-12-01");
        while (m.find()) {
            System.out.println(m.group(1) + " " + m.group(2));
        }
    }
}
```

```output for=collectors
{book=12, ink=10, pen=14}
2024 01
2025 12
```

- Verdict: native. `Collectors.groupingBy` states the grouping and the sum, and the stream library writes the loop. `java.util.regex` compiles patterns into matchers.
- Annotations such as `@Override` and frameworks that read them, such as dependency injection containers, are another declarative layer on top of the language.

```java run id=loop-to-pipeline
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class Main {
    record Order(String email, String country, boolean paid) {}

    // Before: one loop body chooses, changes and collects.
    static List<String> emailsBefore(List<Order> orders, String country) {
        List<String> result = new ArrayList<>();
        for (Order o : orders) {
            if (o.country().equals(country)) {
                if (o.paid()) {
                    String email = o.email().toLowerCase();
                    if (!result.contains(email)) result.add(email);
                }
            }
        }
        Collections.sort(result);
        return result;
    }

    // After: each step is one stream operation.
    static List<String> emails(List<Order> orders, String country) {
        return orders.stream()
                .filter(o -> o.country().equals(country) && o.paid())
                .map(o -> o.email().toLowerCase())
                .distinct()
                .sorted()
                .toList();
    }

    public static void main(String[] args) {
        List<Order> orders = List.of(
                new Order("Ana@example.com", "PT", true),
                new Order("bo@example.com", "SE", true),
                new Order("ana@example.com", "PT", true),
                new Order("cy@example.com", "PT", false),
                new Order("Dee@example.com", "PT", true));
        List<String> want = List.of("ana@example.com", "dee@example.com");
        if (!emailsBefore(orders, "PT").equals(want)) throw new AssertionError("before");
        if (!emails(orders, "PT").equals(want)) throw new AssertionError("after");
        if (!emails(orders, "FR").isEmpty()) throw new AssertionError("empty");
        System.out.println("emails: tests pass");
    }
}
```

```output
emails: tests pass
```

- `distinct` and `sorted` are stream operations, so the pipeline names each of the four steps of the loop.
- `toList` returns an unmodifiable list. The first version returns an `ArrayList` that the caller can change.

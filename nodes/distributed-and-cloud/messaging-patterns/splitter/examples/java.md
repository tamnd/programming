```java run id=split-order
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

public class Main {
    record Line(String kind, int qty) {}

    // One message from the splitter, with the headers that an aggregator needs.
    record Msg(String correlationId, int seq, int size, Line line) {}

    static List<Msg> split(String orderId, List<Line> lines) {
        if (lines.isEmpty()) {
            // One message with size 0, so that a later aggregator is not left waiting.
            return List.of(new Msg(orderId, 0, 0, null));
        }
        List<Msg> out = new ArrayList<>();
        for (int i = 0; i < lines.size(); i++) {
            out.add(new Msg(orderId, i + 1, lines.size(), lines.get(i)));
        }
        return out;
    }

    public static void main(String[] args) {
        Map<String, List<Line>> orders = new LinkedHashMap<>();
        orders.put("A7", List.of(new Line("widget", 2), new Line("gadget", 1), new Line("widget", 5)));
        orders.put("B9", List.of());
        Map<String, String> channel = Map.of("widget", "inventory.widgets", "gadget", "inventory.gadgets");
        orders.forEach((id, lines) -> {
            System.out.printf("order %s has %d lines%n", id, lines.size());
            for (Msg m : split(id, lines)) {
                if (m.line() == null) {
                    System.out.printf("  %s 0/0 empty order -> orders.empty%n", id);
                    continue;
                }
                System.out.printf("  %s %d/%d %s x%d -> %s%n", id, m.seq(), m.size(),
                    m.line().kind(), m.line().qty(), channel.get(m.line().kind()));
            }
        });
    }
}
```

```output
order A7 has 3 lines
  A7 1/3 widget x2 -> inventory.widgets
  A7 2/3 gadget x1 -> inventory.gadgets
  A7 3/3 widget x5 -> inventory.widgets
order B9 has 0 lines
  B9 0/0 empty order -> orders.empty
```

- Verdict: idiom. A loop over the `List` of lines makes one record for each line, with the correlation id, the sequence number and the size.
- A `LinkedHashMap` keeps the order in which the orders were added, so the output has a fixed order.
- In Apache Camel, the same step is `split(body())` in a route, and Camel adds the split index and size as properties of each part.

```java run id=route-orders
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.function.Predicate;

public class Main {
    record Order(String id, String type, int qty) {}

    record Rule(Predicate<Order> test, String channel) {}

    // The router reads one input channel and writes to the channel of the system for each item type.
    static final List<Rule> RULES = List.of(
        new Rule(o -> o.type().equals("widget"), "inventory.widgets"),
        new Rule(o -> o.type().equals("gadget"), "inventory.gadgets"));

    static final Map<String, List<String>> CHANNELS = new LinkedHashMap<>();

    static void route(Order o) {
        var channel = "invalid"; // an unknown type is not lost
        for (var rule : RULES) {
            if (rule.test().test(o)) {
                channel = rule.channel();
                break; // the first rule that matches wins
            }
        }
        CHANNELS.get(channel).add(o.id());
    }

    public static void main(String[] args) {
        for (var name : List.of("inventory.widgets", "inventory.gadgets", "invalid")) {
            CHANNELS.put(name, new ArrayList<>());
        }
        var incoming = List.of(new Order("o1", "widget", 3), new Order("o2", "gadget", 1),
            new Order("o3", "gizmo", 2), new Order("o4", "widget", 1));
        for (var o : incoming) {
            route(o);
        }
        CHANNELS.forEach((name, ids) -> System.out.println(name + ": " + String.join(" ", ids)));
    }
}
```

```output
inventory.widgets: o1 o4
inventory.gadgets: o2
invalid: o3
```

- Verdict: idiom. A list of rules, each a `Predicate` and a channel, is the router; the loop stops at the first rule that matches, as the Choice EIP of Apache Camel does.
- The variable `channel` starts with the name of the invalid channel, so a message that matches no rule goes there.
- A `LinkedHashMap` keeps the order in which the channels were added, so the output has a fixed order.

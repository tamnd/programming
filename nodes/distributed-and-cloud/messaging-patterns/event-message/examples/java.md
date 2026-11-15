```java run id=order-events
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.function.Function;

public class Main {
    // An event message with the required CloudEvents attributes and the data of the event.
    record CloudEvent(String specversion, String id, String source, String type, Map<String, Integer> data) {}

    record Subscriber(String name, Map<String, Function<Map<String, Integer>, String>> handlers, Set<String> seen) {
        void deliver(CloudEvent e) {
            if (!seen.add(e.source() + " " + e.id())) {
                System.out.println(name + ": skip duplicate " + e.id());
                return;
            }
            var handler = handlers.get(e.type());
            if (handler == null) {
                System.out.println(name + ": ignore " + e.type());
                return;
            }
            System.out.println(name + ": " + handler.apply(e.data()));
        }
    }

    static CloudEvent event(String id, String type, Map<String, Integer> data) {
        return new CloudEvent("1.0", id, "/shop", "com.example.order." + type, data);
    }

    public static void main(String[] args) {
        var billing = new Subscriber("billing", Map.of(
            "com.example.order.placed", d -> "charge order " + d.get("order") + ", " + d.get("cents") + " cents",
            "com.example.order.cancelled", d -> "refund order " + d.get("order")), new HashSet<>());
        var email = new Subscriber("email", Map.of(
            "com.example.order.placed", d -> "mail the confirmation of order " + d.get("order")), new HashSet<>());
        var topic = List.of(billing, email); // each subscriber gets a copy of each event

        var events = List.of(
            event("e1", "placed", Map.of("order", 41, "cents", 1999)),
            event("e2", "placed", Map.of("order", 42, "cents", 500)),
            event("e2", "placed", Map.of("order", 42, "cents", 500)), // sent again after a network error
            event("e3", "cancelled", Map.of("order", 41)));
        for (var e : events) {
            for (var s : topic) {
                s.deliver(e);
            }
        }
    }
}
```

```output
billing: charge order 41, 1999 cents
email: mail the confirmation of order 41
billing: charge order 42, 500 cents
email: mail the confirmation of order 42
billing: skip duplicate e2
email: skip duplicate e2
billing: refund order 41
email: ignore com.example.order.cancelled
```

- Verdict: idiom. Java has no publish-subscribe channel in the standard library and no JSON, so the events here are records in memory. A Jakarta Messaging topic gives the same structure between processes.
- `Set.add` returns `false` when the key is already in the set, so one call both finds and records a duplicate.
- `Map.of` gives a map that cannot change, which suits a fixed set of handlers.

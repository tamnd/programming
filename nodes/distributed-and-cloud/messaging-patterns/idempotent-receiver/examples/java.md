```java run id=payment-receiver
import java.util.*;

public class Main {
    static final int WINDOW = 3;
    record Delivery(String id, int order, int cents) {}

    static class PaymentReceiver {
        // insertion order; the oldest id is removed when the window is full
        final Map<String, Boolean> seen = new LinkedHashMap<>() {
            @Override
            protected boolean removeEldestEntry(Map.Entry<String, Boolean> eldest) {
                return size() > WINDOW;
            }
        };
        final Map<Integer, Integer> totals = new HashMap<>();
        final Set<String> ever = new HashSet<>(); // only to explain the output

        String receive(Delivery d) {
            if (seen.containsKey(d.id())) return d.id() + ": duplicate, ignored";
            int total = totals.merge(d.order(), d.cents(), Integer::sum);
            seen.put(d.id(), true);
            String note = ever.add(d.id()) ? ""
                    : " (applied twice: the id left the window of " + WINDOW + " ids)";
            return d.id() + ": order " + d.order() + " +" + d.cents() + ", total " + total + note;
        }
    }

    public static void main(String[] args) {
        List<Delivery> deliveries = List.of(
                new Delivery("pay-1", 17, 1250), new Delivery("pay-2", 18, 800), new Delivery("pay-1", 17, 1250),
                new Delivery("pay-3", 17, 300), new Delivery("pay-4", 19, 500), new Delivery("pay-5", 20, 100),
                new Delivery("pay-2", 18, 800));
        PaymentReceiver r = new PaymentReceiver();
        for (Delivery d : deliveries) System.out.println(r.receive(d));
    }
}
```

```output
pay-1: order 17 +1250, total 1250
pay-2: order 18 +800, total 800
pay-1: duplicate, ignored
pay-3: order 17 +300, total 1550
pay-4: order 19 +500, total 500
pay-5: order 20 +100, total 100
pay-2: order 18 +800, total 1600 (applied twice: the id left the window of 3 ids)
```

- Verdict: idiom. A `LinkedHashMap` that overrides `removeEldestEntry` is a bounded store of ids in a few lines.
- `totals.merge(order, cents, Integer::sum)` adds to the total of an order, or starts it.
- The set `ever` exists only to explain the output; `ever.add` returns `false` for an id that came before.

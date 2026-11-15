```java run id=bridge-queues
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.stream.Collectors;

public class Main {
    record SourceMsg(String id, String channel, String label, int size) {}
    record DestMsg(String id, String subject, int size) {}

    static final int LIMIT = 64 * 1024;

    public static void main(String[] args) {
        Map<String, String> routes = Map.of("orders", "sb-orders", "invoices", "sb-invoices");
        var source = new ArrayDeque<>(List.of(
                new SourceMsg("m1", "orders", "new-order", 120), new SourceMsg("m2", "invoices", "invoice", 300),
                new SourceMsg("m3", "orders", "new-order", 90), new SourceMsg("m4", "orders", "bulk-order", 70000),
                new SourceMsg("m5", "invoices", "invoice", 210)));
        Map<String, List<DestMsg>> dest = new LinkedHashMap<>();
        dest.put("sb-orders", new ArrayList<>());
        dest.put("sb-invoices", new ArrayList<>());
        List<String> errors = new ArrayList<>();
        Set<String> forwarded = new HashSet<>();
        boolean crashed = false;

        while (!source.isEmpty()) {
            SourceMsg m = source.peekFirst(); // the message stays on the source until the ack
            if (m.size() > LIMIT) {
                System.out.printf("%s too large for the destination (%d > %d bytes): to the bridge error queue%n",
                        m.id(), m.size(), LIMIT);
                errors.add(m.id());
            } else if (forwarded.contains(m.id())) {
                System.out.printf("%s again: already forwarded, ack only%n", m.id());
            } else {
                String target = routes.get(m.channel());
                dest.get(target).add(new DestMsg(m.id(), m.label(), m.size()));
                forwarded.add(m.id());
                System.out.printf("%s %s -> %s, %d bytes, subject %s%n", m.id(), m.channel(), target, m.size(), m.label());
                if (m.id().equals("m3") && !crashed) {
                    crashed = true;
                    System.out.println("the bridge stops before the ack of m3; the source delivers m3 again");
                    continue;
                }
            }
            source.pollFirst(); // the ack
        }

        String summary = dest.entrySet().stream()
                .map(e -> e.getKey() + ": " + e.getValue().stream().map(DestMsg::id).collect(Collectors.joining(" ")))
                .collect(Collectors.joining(" | "));
        System.out.println(summary + " | bridge errors: " + String.join(" ", errors));
    }
}
```

```output
m1 orders -> sb-orders, 120 bytes, subject new-order
m2 invoices -> sb-invoices, 300 bytes, subject invoice
m3 orders -> sb-orders, 90 bytes, subject new-order
the bridge stops before the ack of m3; the source delivers m3 again
m3 again: already forwarded, ack only
m4 too large for the destination (70000 > 65536 bytes): to the bridge error queue
m5 invoices -> sb-invoices, 210 bytes, subject invoice
sb-orders: m1 m3 | sb-invoices: m2 m5 | bridge errors: m4
```

- Verdict: idiom. An `ArrayDeque` is the source queue; `peekFirst` reads the head, and `pollFirst` after the forward is the ack.
- Records hold the source and the destination formats, so the map from `label` to `subject` is one constructor call.
- A `LinkedHashMap` keeps the destination channels in a fixed order for the summary line.

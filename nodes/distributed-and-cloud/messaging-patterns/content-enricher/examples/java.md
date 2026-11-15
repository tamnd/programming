```java run id=enrich-orders
import java.time.Clock;
import java.time.Duration;
import java.time.Instant;
import java.time.ZoneOffset;
import java.time.format.DateTimeFormatter;
import java.util.List;
import java.util.Map;
import java.util.Optional;

public class Main {
    record Order(String id, String customer, String zip) {}
    record Enriched(Order order, String name, String state, Instant received) {}

    static final Map<String, String> ZIP_PREFIX = Map.of("981", "WA", "100", "NY", "606", "IL");
    static final Map<String, String> DIRECTORY = Map.of("c1", "Ada Park", "c2", "Lin Tran");

    static class Enricher {
        Clock clock = Clock.fixed(Instant.parse("2026-01-05T09:00:00Z"), ZoneOffset.UTC);
        int lookups = 0;

        Optional<Enriched> enrich(Order o) {
            lookups++;
            String name = DIRECTORY.get(o.customer());
            if (name == null) {
                return Optional.empty();
            }
            clock = Clock.offset(clock, Duration.ofSeconds(1));
            String state = ZIP_PREFIX.getOrDefault(o.zip().substring(0, 3), "??");
            return Optional.of(new Enriched(o, name, state, clock.instant()));
        }
    }

    public static void main(String[] args) {
        var fmt = DateTimeFormatter.ofPattern("HH:mm:ss").withZone(ZoneOffset.UTC);
        var incoming = List.of(new Order("o1", "c1", "98101"), new Order("o2", "c2", "10001"),
                new Order("o3", "c1", "60601"), new Order("o4", "c9", "98101"));
        var e = new Enricher();
        int enriched = 0, invalid = 0;
        for (Order o : incoming) {
            var m = e.enrich(o);
            if (m.isEmpty()) {
                invalid++;
                System.out.println("invalid: " + o.id() + " customer " + o.customer() + " not found");
                continue;
            }
            enriched++;
            var r = m.get();
            System.out.printf("%s %s %s, %s %s, received %s%n", o.id(), o.customer(), r.name(),
                    o.zip(), r.state(), fmt.format(r.received()));
        }
        System.out.printf("lookups: %d, enriched: %d, invalid: %d%n", e.lookups, enriched, invalid);
    }
}
```

```output
o1 c1 Ada Park, 98101 WA, received 09:00:01
o2 c2 Lin Tran, 10001 NY, received 09:00:02
o3 c1 Ada Park, 60601 IL, received 09:00:03
invalid: o4 customer c9 not found
lookups: 4, enriched: 3, invalid: 1
```

- Verdict: idiom. Records hold the order and the enriched message, and `Map.of` stands for the directory and the ZIP table.
- `java.time.Clock.fixed` and `Clock.offset` give a clock that the program controls, so the output is the same on each run.
- `Optional.empty()` marks a miss, and the caller reports the order as invalid.

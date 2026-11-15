```java run id=resequence
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.StringJoiner;
import java.util.TreeMap;

public class Main {
    record Msg(String key, int seq) {
    }

    public static void main(String[] args) {
        // Messages arrive with a key and a sequence number for each key, out of order and with one copy.
        List<Msg> arrivals = List.of(new Msg("A", 1), new Msg("A", 3), new Msg("B", 1), new Msg("A", 2),
                new Msg("B", 3), new Msg("A", 3), new Msg("B", 2), new Msg("A", 4));

        Map<String, Integer> expected = new TreeMap<>(); // key -> next sequence number to deliver
        Map<String, Map<Integer, String>> held = new HashMap<>(); // key -> waiting messages
        for (Msg m : arrivals) {
            int next = expected.getOrDefault(m.key(), 1);
            String name = m.key() + m.seq();
            Map<Integer, String> h = held.computeIfAbsent(m.key(), k -> new HashMap<>());
            if (m.seq() < next || h.containsKey(m.seq())) {
                System.out.println(name + ": copy, dropped");
                continue;
            }
            h.put(m.seq(), name);
            List<String> out = new ArrayList<>();
            while (h.containsKey(next)) {
                out.add(h.remove(next));
                next++;
            }
            expected.put(m.key(), next);
            if (out.isEmpty()) {
                System.out.println(name + ": hold, waiting for " + m.key() + next);
            } else {
                System.out.println(name + ": deliver " + String.join(" ", out));
            }
        }
        int left = held.values().stream().mapToInt(Map::size).sum();
        StringJoiner nexts = new StringJoiner(", ");
        expected.forEach((k, v) -> nexts.add(k + v));
        System.out.println("held at the end: " + left + "; next: " + nexts);
    }
}
```

```output
A1: deliver A1
A3: hold, waiting for A2
B1: deliver B1
A2: deliver A2 A3
B3: hold, waiting for B2
A3: copy, dropped
B2: deliver B2 B3
A4: deliver A4
held at the end: 0; next: A5, B4
```

- Verdict: idiom. A `TreeMap` holds the next number for each key, so the keys print in sorted order, and a `HashMap` for each key holds the early messages.
- `computeIfAbsent` makes the map of held messages the first time that a key arrives.
- `containsKey` tests for the next message, and `remove` returns it as it takes it out of the map.

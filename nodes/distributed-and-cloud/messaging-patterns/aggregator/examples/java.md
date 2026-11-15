```java run id=aggregate-parts
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;

public class Main {
    // A message from a splitter.
    record Msg(String cid, int seq, int size, String item) {}

    public static void main(String[] args) {
        List<Msg> incoming = List.of(
            new Msg("A", 2, 3, "gadget"),
            new Msg("B", 1, 2, "widget"),
            new Msg("A", 1, 3, "widget"),
            new Msg("B", 2, 2, "gizmo"),
            new Msg("A", 2, 3, "gadget"), // a second copy, as at-least-once delivery can give
            new Msg("A", 3, 3, "widget"),
            new Msg("B", 1, 2, "widget")); // a copy that comes after B is complete

        Map<String, TreeMap<Integer, String>> groups = new HashMap<>();
        Set<String> closed = new HashSet<>();

        for (Msg m : incoming) {
            if (closed.contains(m.cid())) {
                System.out.printf("late: %s %d/%d discarded, the group is closed%n", m.cid(), m.seq(), m.size());
                continue;
            }
            TreeMap<Integer, String> parts = groups.computeIfAbsent(m.cid(), k -> new TreeMap<>());
            if (parts.putIfAbsent(m.seq(), m.item()) != null) {
                System.out.printf("duplicate: %s %d/%d ignored%n", m.cid(), m.seq(), m.size());
                continue;
            }
            if (parts.size() == m.size()) { // the completion condition: all parts are here
                System.out.printf("%s complete: %s%n", m.cid(), String.join(", ", parts.values()));
                groups.remove(m.cid());
                closed.add(m.cid());
            }
        }
        System.out.printf("still open: %d%n", groups.size());
    }
}
```

```output
B complete: widget, gizmo
duplicate: A 2/3 ignored
A complete: widget, gadget, widget
late: B 1/2 discarded, the group is closed
still open: 0
```

- Verdict: idiom. A `HashMap` from the correlation id to a `TreeMap` of the parts is the store; the `TreeMap` keeps the parts in the order of their sequence numbers.
- `putIfAbsent` returns the old value for a second copy, so the test for a duplicate and the insert are one call.
- Apache Camel and Spring Integration give the same parts as options: a correlation expression, a completion size or a release strategy, and a store of groups.

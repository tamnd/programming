```java run id=prio-dispatch
import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;
import java.util.Map;
import java.util.PriorityQueue;

public class Main {
    record Arrival(int tick, String id, String prio) {
    }

    record Msg(int level, long seq, int arrived, String id, String prio) {
    }

    public static void main(String[] args) {
        Map<String, Integer> level = Map.of("high", 0, "normal", 1, "low", 2);
        List<Arrival> arrivals = List.of(new Arrival(0, "l1", "low"), new Arrival(0, "l2", "low"),
                new Arrival(0, "h1", "high"), new Arrival(1, "n1", "normal"), new Arrival(1, "h2", "high"),
                new Arrival(2, "l3", "low"), new Arrival(3, "h3", "high"), new Arrival(4, "n2", "normal"));
        // PriorityQueue is not stable, so the arrival number breaks ties.
        PriorityQueue<Msg> queue = new PriorityQueue<>(
                Comparator.comparingInt(Msg::level).thenComparingLong(Msg::seq));
        List<String> order = new ArrayList<>();
        long seq = 0;
        int i = 0;
        for (int t = 0; i < arrivals.size() || !queue.isEmpty(); t++) {
            while (i < arrivals.size() && arrivals.get(i).tick() == t) {
                Arrival a = arrivals.get(i++);
                queue.add(new Msg(level.get(a.prio()), seq++, t, a.id(), a.prio()));
            }
            Msg m = queue.poll();
            if (m != null) {
                System.out.printf("tick %d: %s (%s, waited %d)%n", t, m.id(), m.prio(), t - m.arrived());
                order.add(m.id());
            }
        }
        System.out.println("order: " + String.join(" ", order));
        System.out.println("l1 before l2: " + (order.indexOf("l1") < order.indexOf("l2")));
    }
}
```

```output
tick 0: h1 (high, waited 0)
tick 1: h2 (high, waited 0)
tick 2: n1 (normal, waited 1)
tick 3: h3 (high, waited 0)
tick 4: n2 (normal, waited 0)
tick 5: l1 (low, waited 5)
tick 6: l2 (low, waited 6)
tick 7: l3 (low, waited 5)
order: h1 h2 n1 h3 n2 l1 l2 l3
l1 before l2: true
```

- Verdict: idiom. `java.util.PriorityQueue` is a heap, and `Comparator.comparingInt(...).thenComparingLong(...)` builds the key of level and arrival number.
- The Javadoc of `PriorityQueue` says that ties are broken arbitrarily, so the arrival number is necessary to keep FIFO order inside a priority.
- A record holds each message, so the comparator reads the fields through the accessor methods.

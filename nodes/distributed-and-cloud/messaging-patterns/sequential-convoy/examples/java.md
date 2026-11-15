```java run id=order-sessions
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

public class Main {
    record Op(String name, String arg) {
        String label(String sid) {
            return arg.isEmpty() ? sid + " " + name : sid + " " + name + " " + arg;
        }
    }

    static final int MAX_DELIVERIES = 3;

    public static void main(String[] args) {
        String[][] arrivals = {{"A", "create", ""}, {"B", "create", ""}, {"C", "create", ""}, {"A", "add", "10"},
                {"B", "add", "x"}, {"A", "add", "5"}, {"C", "add", "3"}, {"B", "add", "7"}};
        Map<String, ArrayDeque<Op>> sessions = new LinkedHashMap<>(); // keeps the order of first arrival
        for (String[] a : arrivals) {
            sessions.computeIfAbsent(a[0], k -> new ArrayDeque<>()).add(new Op(a[1], a[2]));
        }
        Map<String, String> holding = new LinkedHashMap<>();
        holding.put("w1", null);
        holding.put("w2", null);
        Map<String, String> locks = new HashMap<>();
        Map<String, Integer> deliveries = new HashMap<>();
        Map<String, Integer> totals = new LinkedHashMap<>();
        List<String> dead = new ArrayList<>();
        int outOfOrder = 0;

        for (int rnd = 1; sessions.values().stream().anyMatch(q -> !q.isEmpty()); rnd++) {
            for (String w : holding.keySet()) {
                if (holding.get(w) == null) {
                    String free = sessions.entrySet().stream()
                            .filter(e -> !e.getValue().isEmpty() && !locks.containsKey(e.getKey()))
                            .map(Map.Entry::getKey).findFirst().orElse(null);
                    if (free == null) {
                        continue;
                    }
                    locks.put(free, w);
                    holding.put(w, free);
                    System.out.printf("%d %s locks %s%n", rnd, w, free);
                }
                String sid = holding.get(w);
                Op op = sessions.get(sid).peekFirst();
                try {
                    if (op.name().equals("create")) {
                        totals.put(sid, 0);
                    } else if (!totals.containsKey(sid)) {
                        outOfOrder++;
                    } else {
                        totals.merge(sid, Integer.parseInt(op.arg()), Integer::sum);
                    }
                    System.out.printf("%d %s %s%n", rnd, w, op.label(sid));
                    sessions.get(sid).pollFirst();
                    deliveries.remove(sid);
                } catch (NumberFormatException e) {
                    int n = deliveries.merge(sid, 1, Integer::sum);
                    if (n < MAX_DELIVERIES) {
                        System.out.printf("%d %s %s failed, delivery %d%n", rnd, w, op.label(sid), n);
                    } else {
                        System.out.printf("%d %s %s failed, delivery %d: to the dead letter queue%n", rnd, w, op.label(sid), n);
                        dead.add(op.label(sid));
                        sessions.get(sid).pollFirst();
                        deliveries.remove(sid);
                    }
                }
                if (sessions.get(sid).isEmpty()) {
                    locks.remove(sid);
                    holding.put(w, null);
                }
            }
        }

        var parts = new ArrayList<String>();
        totals.forEach((k, v) -> parts.add(k + " " + v));
        System.out.printf("totals: %s; dead letter: %s; out of order: %d%n",
                String.join(", ", parts), String.join(", ", dead), outOfOrder);
    }
}
```

```output
1 w1 locks A
1 w1 A create
1 w2 locks B
1 w2 B create
2 w1 A add 10
2 w2 B add x failed, delivery 1
3 w1 A add 5
3 w2 B add x failed, delivery 2
4 w1 locks C
4 w1 C create
4 w2 B add x failed, delivery 3: to the dead letter queue
5 w1 C add 3
5 w2 B add 7
totals: A 15, B 7, C 3; dead letter: B add x; out of order: 0
```

- Verdict: idiom. A `LinkedHashMap` of `ArrayDeque` objects holds the sessions and keeps the order of first arrival.
- A record `Op` is one operation, and `computeIfAbsent` makes a session when its first message comes.
- `Integer.parseInt` throws `NumberFormatException` for `x`; the catch block counts the delivery and leaves the message at the head of its session.

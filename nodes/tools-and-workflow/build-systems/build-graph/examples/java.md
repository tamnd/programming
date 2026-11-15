```java run id=graph
// A build graph: a topological order, the waves of a parallel build, and a cycle check.
import java.util.*;

public class Main {
    static final Map<String, List<String>> RULES = Map.of(
        "app", List.of("main.o", "parse.o", "lex.o"),
        "main.o", List.of("main.c", "parse.h"),
        "parse.o", List.of("parse.c", "parse.h", "lex.h"),
        "lex.o", List.of("lex.c", "lex.h"),
        "parse.c", List.of("grammar.y"),
        "parse.h", List.of("grammar.y"));

    static SortedSet<String> nodes(Map<String, List<String>> rules) {
        SortedSet<String> names = new TreeSet<>(rules.keySet());
        rules.values().forEach(names::addAll);
        return names;
    }

    // Kahn's algorithm: a node is ready when all its dependencies are done.
    static List<String> topologicalOrder(Map<String, List<String>> rules) {
        Map<String, Integer> waiting = new HashMap<>();
        Map<String, List<String>> users = new HashMap<>();
        TreeSet<String> ready = new TreeSet<>();
        for (String n : nodes(rules)) {
            waiting.put(n, rules.getOrDefault(n, List.of()).size());
            users.put(n, new ArrayList<>());
            if (waiting.get(n) == 0) {
                ready.add(n);
            }
        }
        rules.forEach((target, deps) -> deps.forEach(dep -> users.get(dep).add(target)));
        List<String> order = new ArrayList<>();
        while (!ready.isEmpty()) {
            String n = ready.pollFirst();
            order.add(n);
            for (String user : users.get(n)) {
                if (waiting.merge(user, -1, Integer::sum) == 0) {
                    ready.add(user);
                }
            }
        }
        return order;
    }

    static List<List<String>> waves(Map<String, List<String>> rules) {
        Map<String, Integer> level = new HashMap<>();
        TreeMap<Integer, List<String>> out = new TreeMap<>();
        for (String n : topologicalOrder(rules)) {
            int k = 0;
            for (String dep : rules.getOrDefault(n, List.of())) {
                k = Math.max(k, level.get(dep) + 1);
            }
            level.put(n, k);
            if (k > 0) {
                out.computeIfAbsent(k, x -> new ArrayList<>()).add(n);
            }
        }
        List<List<String>> result = new ArrayList<>();
        for (List<String> wave : out.values()) {
            Collections.sort(wave);
            result.add(wave);
        }
        return result;
    }

    static List<String> findCycle(Map<String, List<String>> rules) {
        Map<String, Integer> state = new HashMap<>();
        Deque<String> path = new ArrayDeque<>();
        for (String n : nodes(rules)) {
            if (!state.containsKey(n)) {
                List<String> found = visit(n, rules, state, path);
                if (found != null) {
                    return found;
                }
            }
        }
        return null;
    }

    static List<String> visit(String n, Map<String, List<String>> rules,
                              Map<String, Integer> state, Deque<String> path) {
        state.put(n, 1);
        path.addLast(n);
        for (String dep : rules.getOrDefault(n, List.of())) {
            if (state.getOrDefault(dep, 0) == 1) {
                List<String> list = new ArrayList<>(path);
                List<String> cycle = new ArrayList<>(list.subList(list.indexOf(dep), list.size()));
                cycle.add(dep);
                return cycle;
            }
            if (!state.containsKey(dep)) {
                List<String> found = visit(dep, rules, state, path);
                if (found != null) {
                    return found;
                }
            }
        }
        path.removeLast();
        state.put(n, 2);
        return null;
    }

    public static void main(String[] args) {
        System.out.println("order: " + String.join(" ", topologicalOrder(RULES)));
        List<List<String>> waves = waves(RULES);
        for (int i = 0; i < waves.size(); i++) {
            System.out.println("wave " + (i + 1) + ": " + String.join(" ", waves.get(i)));
        }
        List<String> found = findCycle(RULES);
        System.out.println("cycle: " + (found == null ? "none" : String.join(" -> ", found)));
        Map<String, List<String>> bad = new HashMap<>(RULES);
        bad.put("lex.h", List.of("lex.o"));
        System.out.println("after lex.h <- lex.o:");
        List<String> order = topologicalOrder(bad);
        System.out.println("order: " + String.join(" ", order) + " (" + order.size() + " of " + nodes(bad).size() + " nodes)");
        System.out.println("cycle: " + String.join(" -> ", findCycle(bad)));
    }
}
```

```output
order: grammar.y lex.c lex.h lex.o main.c parse.c parse.h main.o parse.o app
wave 1: lex.o parse.c parse.h
wave 2: main.o parse.o
wave 3: app
cycle: none
after lex.h <- lex.o:
order: grammar.y lex.c main.c parse.c parse.h main.o (6 of 10 nodes)
cycle: lex.h -> lex.o -> lex.h
```

- Verdict: idiom. `Map.of` holds the rules, and a `TreeSet` holds the ready nodes, so `pollFirst` gives the node with the smallest name.
- `waiting.merge(user, -1, Integer::sum)` lowers the count and returns the lowered count in one call.
- The search keeps the current path in an `ArrayDeque` and copies it to a list to cut out the cycle.

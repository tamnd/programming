```java run id=models
// Two build systems from the same parts: a scheduler and a rebuilder.
// The tasks are a small spreadsheet. B3 has a dynamic dependency.
import java.util.*;
import java.util.function.Function;

public class Main {
    interface Task {
        int run(Function<String, Integer> fetch);
    }

    static final Map<String, Task> TASKS = Map.of(
        "B1", f -> f.apply("A1") + f.apply("A2"),
        "B2", f -> f.apply("B1") * 2,
        "B3", f -> f.apply("C1") == 1 ? f.apply("B2") : f.apply("A2"));
    // A topological scheduler must know the dependencies before the build.
    // For B3 it must list both branches.
    static final Map<String, List<String>> STATIC_DEPS = Map.of(
        "B1", List.of("A1", "A2"), "B2", List.of("B1"), "B3", List.of("C1", "B2", "A2"));
    static final List<String> TARGETS = List.of("B2", "B3");

    interface BuildSystem {
        void setInput(String key, int value);
        List<String> build();
        Map<String, Integer> store();
    }

    /** Topological scheduler with a dirty bit. */
    static class MakeLike implements BuildSystem {
        final Map<String, Integer> store = new HashMap<>();
        final Set<String> dirty = new HashSet<>();

        public void setInput(String key, int value) {
            store.put(key, value);
            dirty.add(key); // writing a file sets its time, even with the same content
        }

        void visit(String k, Set<String> seen, List<String> order) {
            if (!TASKS.containsKey(k) || !seen.add(k)) return;
            for (String d : STATIC_DEPS.get(k)) visit(d, seen, order);
            order.add(k);
        }

        public List<String> build() {
            List<String> order = new ArrayList<>();
            Set<String> seen = new HashSet<>();
            for (String t : TARGETS) visit(t, seen, order);
            List<String> ran = new ArrayList<>();
            for (String k : order) {
                boolean changed = STATIC_DEPS.get(k).stream().anyMatch(dirty::contains);
                if (!store.containsKey(k) || changed) {
                    store.put(k, TASKS.get(k).run(store::get));
                    dirty.add(k);
                    ran.add(k);
                }
            }
            dirty.clear();
            return ran;
        }

        public Map<String, Integer> store() { return store; }
    }

    record Dep(String key, String hash) {}
    record Trace(List<Dep> deps, String result) {}

    /** Suspending scheduler with verifying traces. */
    static class ShakeLike implements BuildSystem {
        final Map<String, Integer> store = new HashMap<>();
        final Map<String, Trace> traces = new HashMap<>();
        Set<String> done;
        List<String> ran;

        public void setInput(String key, int value) { store.put(key, value); }

        int fetch(String k) {
            if (!TASKS.containsKey(k) || done.contains(k)) return store.get(k);
            Trace t = traces.get(k);
            if (t != null && String.valueOf(store.get(k)).equals(t.result())) {
                boolean upToDate = true;
                for (Dep d : t.deps()) {
                    if (!String.valueOf(fetch(d.key())).equals(d.hash())) { upToDate = false; break; }
                }
                if (upToDate) { done.add(k); return store.get(k); }
            }
            List<Dep> deps = new ArrayList<>();
            int v = TASKS.get(k).run(d -> {
                // The task waits here until d is up to date: this is the suspending scheduler.
                int dv = fetch(d);
                deps.add(new Dep(d, String.valueOf(dv)));
                return dv;
            });
            store.put(k, v);
            traces.put(k, new Trace(deps, String.valueOf(v)));
            done.add(k);
            ran.add(k);
            return v;
        }

        public List<String> build() {
            done = new HashSet<>();
            ran = new ArrayList<>();
            for (String t : TARGETS) fetch(t);
            return ran;
        }

        public Map<String, Integer> store() { return store; }
    }

    static String show(List<String> ran) {
        return ran.isEmpty() ? "-" : String.join(" ", ran);
    }

    public static void main(String[] args) {
        BuildSystem make = new MakeLike(), shake = new ShakeLike();
        for (BuildSystem b : List.of(make, shake)) {
            b.setInput("A1", 10); b.setInput("A2", 20); b.setInput("C1", 1);
        }
        Object[][] steps = {
            {"clean build", Map.of()},
            {"A2 = 20 again", Map.of("A2", 20)},
            {"A1 = 11, A2 = 19", Map.of("A1", 11, "A2", 19)},
            {"C1 = 2", Map.of("C1", 2)},
            {"A1 = 12", Map.of("A1", 12)},
        };
        System.out.printf("%-18s%-14s%s%n", "step", "make-like", "shake-like");
        for (Object[] step : steps) {
            @SuppressWarnings("unchecked")
            Map<String, Integer> changes = (Map<String, Integer>) step[1];
            for (BuildSystem b : List.of(make, shake)) changes.forEach(b::setInput);
            System.out.printf("%-18s%-14s%s%n", step[0], show(make.build()), show(shake.build()));
        }
        boolean same = TASKS.keySet().stream().allMatch(k -> make.store().get(k).equals(shake.store().get(k)));
        System.out.println("same values: " + same);
        Map<String, Integer> s = shake.store();
        System.out.println("B1, B2, B3 = " + s.get("B1") + " " + s.get("B2") + " " + s.get("B3"));
    }
}
```

```output
step              make-like     shake-like
clean build       B1 B2 B3      B1 B2 B3
A2 = 20 again     B1 B2 B3      -
A1 = 11, A2 = 19  B1 B2 B3      B1
C1 = 2            B3            B3
A1 = 12           B1 B2 B3      B1 B2
same values: true
B1, B2, B3 = 31 62 19
```

- Verdict: idiom. A task is a functional interface that takes a `Function<String, Integer>` as its `fetch` callback, and the tasks are lambdas in a `Map.of`.
- The Make-like system passes `store::get` as `fetch`, so a task reads the store directly. The Shake-like system passes a lambda that first brings the key up to date with `fetch`, then records it.
- `record Dep` and `record Trace` hold the verifying trace: the dependencies with their hashes, and the hash of the result.

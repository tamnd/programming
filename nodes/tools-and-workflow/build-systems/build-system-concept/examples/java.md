```java run id=tiny-build
// A tiny build system: rules, dependencies first, and a skip when a target is up to date.
import java.util.*;

public class Main {
    record Rule(List<String> deps, String action) {}
    record Entry(String value, int time) {}

    static final Map<String, Rule> RULES = Map.of(
        "app", new Rule(List.of("main.o", "util.o"), "link"),
        "main.o", new Rule(List.of("main.c", "util.h"), "compile"),
        "util.o", new Rule(List.of("util.c", "util.h"), "compile"));
    static final Map<String, Entry> store = new HashMap<>();
    static int clock = 0;

    static void write(String key, String value) {
        clock++;
        store.put(key, new Entry(value, clock));
    }

    static void build(String target, Set<String> done, List<String> ran) {
        if (!done.add(target)) {
            return;
        }
        Rule rule = RULES.get(target);
        if (rule == null) {
            if (!store.containsKey(target)) {
                throw new NoSuchElementException("no rule to make " + target);
            }
            return;
        }
        for (String dep : rule.deps()) {
            build(dep, done, ran);
        }
        Entry old = store.get(target);
        boolean dirty = old == null
            || rule.deps().stream().anyMatch(dep -> store.get(dep).time() > old.time());
        if (dirty) {
            write(target, rule.action() + "(" + String.join(", ", rule.deps()) + ")");
            ran.add(rule.action() + " " + target);
        }
    }

    static void run(String label) {
        List<String> ran = new ArrayList<>();
        try {
            build("app", new HashSet<>(), ran);
        } catch (NoSuchElementException e) {
            System.out.println(label + ": error: " + e.getMessage());
            return;
        }
        System.out.println(label + ": " + (ran.isEmpty() ? "nothing to do" : String.join(", ", ran)));
    }

    public static void main(String[] args) {
        for (String name : List.of("main.c", "util.c", "util.h")) {
            write(name, "source of " + name);
        }
        run("build 1");
        run("build 2");
        System.out.println("edit util.h");
        write("util.h", "edited");
        run("build 3");
        System.out.println("edit main.c");
        write("main.c", "edited");
        run("build 4");
        System.out.println("delete util.c");
        store.remove("util.c");
        run("build 5");
    }
}
```

```output
build 1: compile main.o, compile util.o, link app
build 2: nothing to do
edit util.h
build 3: compile main.o, compile util.o, link app
edit main.c
build 4: compile main.o, link app
delete util.c
build 5: error: no rule to make util.c
```

- Verdict: idiom. Records hold a rule and a store entry, and `Map.of` holds the rules, which do not change.
- `done.add(target)` returns false when the target is already in the set, so one call both checks and marks the target.
- A missing input throws `NoSuchElementException`, and `run` catches it to print the error.

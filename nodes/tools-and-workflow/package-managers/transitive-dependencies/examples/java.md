```java run id=dependency-tree
// Print a dependency tree, then find the packages that are shared (a diamond) and the packages with two versions.
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.TreeMap;
import java.util.TreeSet;

public class Main {
    static final Map<String, List<String>> DEPS = Map.of(
            "app", List.of("cli 1.0", "web 2.0"),
            "cli 1.0", List.of("json 0.9", "log 1.2"),
            "web 2.0", List.of("http 1.4", "json 1.1", "log 1.2"),
            "http 1.4", List.of("log 1.2"),
            "json 0.9", List.of(),
            "json 1.1", List.of(),
            "log 1.2", List.of());
    static final Set<String> SEEN = new TreeSet<>();

    static void show(String pkg, String prefix) {
        List<String> children = DEPS.get(pkg);
        for (int i = 0; i < children.size(); i++) {
            String child = children.get(i);
            boolean last = i == children.size() - 1;
            String branch = last ? "`-- " : "+-- ";
            if (SEEN.contains(child)) {
                System.out.println(prefix + branch + child + " (*)");
                continue;
            }
            System.out.println(prefix + branch + child);
            SEEN.add(child);
            show(child, prefix + (last ? "    " : "|   "));
        }
    }

    public static void main(String[] args) {
        System.out.println("app");
        show("app", "");
        Set<String> transitive = new TreeSet<>(SEEN);
        transitive.removeAll(DEPS.get("app"));
        System.out.println("direct: " + String.join(", ", DEPS.get("app")));
        System.out.println("transitive: " + String.join(", ", transitive));

        Map<String, Set<String>> users = new TreeMap<>();
        DEPS.forEach((pkg, children) -> children.forEach(
                child -> users.computeIfAbsent(child, c -> new TreeSet<>()).add(pkg)));
        Map<String, List<String>> versions = new TreeMap<>();
        for (String p : SEEN) {
            String[] parts = p.split(" ");
            versions.computeIfAbsent(parts[0], n -> new ArrayList<>()).add(parts[1]);
        }
        System.out.println("needed by more than one package:");
        users.forEach((p, who) -> {
            if (who.size() > 1) {
                System.out.println("  " + p + ": " + String.join(", ", who));
            }
        });
        System.out.println("more than one version:");
        versions.forEach((name, list) -> {
            if (list.size() > 1) {
                System.out.println("  " + name + ": " + String.join(", ", list));
            }
        });
    }
}
```

```output
app
+-- cli 1.0
|   +-- json 0.9
|   `-- log 1.2
`-- web 2.0
    +-- http 1.4
    |   `-- log 1.2 (*)
    +-- json 1.1
    `-- log 1.2 (*)
direct: cli 1.0, web 2.0
transitive: http 1.4, json 0.9, json 1.1, log 1.2
needed by more than one package:
  log 1.2: cli 1.0, http 1.4, web 2.0
more than one version:
  json: 0.9, 1.1
```

- Verdict: idiom. `Map.of` holds the graph, and a `TreeSet` keeps the shown packages in order.
- `removeAll` on a copy of the set gives the transitive dependencies.
- `computeIfAbsent` builds the map from a package to its users, and a `TreeMap` keeps the output in order.

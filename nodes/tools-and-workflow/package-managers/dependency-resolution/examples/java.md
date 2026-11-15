```java run id=backtracking-resolver
// A backtracking dependency resolver: try the newest version first, and go back on a conflict.
import java.util.ArrayList;
import java.util.Comparator;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;
import java.util.stream.Collectors;

public class Main {
    // who needs name in the range [lo, hi).
    record Req(String name, String lo, String hi, String who) {}

    static final Map<String, Map<String, List<Req>>> VERSIONS = Map.of(
            "web", Map.of(
                    "2.0", List.of(new Req("log", "2.0", "3.0", "")),
                    "1.1", List.of(new Req("log", "1.1", "2.0", "")),
                    "1.0", List.of(new Req("log", "1.0", "2.0", ""))),
            "log", Map.of("2.0", List.of(), "1.2", List.of(), "1.1", List.of(), "1.0", List.of()));

    static int cmp(String a, String b) {
        String[] pa = a.split("\\."), pb = b.split("\\.");
        for (int i = 0; i < pa.length; i++) {
            int d = Integer.parseInt(pa[i]) - Integer.parseInt(pb[i]);
            if (d != 0) {
                return d;
            }
        }
        return 0;
    }

    static boolean fits(String v, String lo, String hi) {
        return cmp(lo, v) <= 0 && cmp(v, hi) < 0;
    }

    static TreeMap<String, String> solve(List<Req> todo, TreeMap<String, String> chosen) {
        if (todo.isEmpty()) {
            return chosen;
        }
        Req r = todo.get(0);
        List<Req> rest = todo.subList(1, todo.size());
        String v = chosen.get(r.name());
        if (v != null) {
            if (fits(v, r.lo(), r.hi())) {
                return solve(rest, chosen);
            }
            System.out.printf("  conflict: %s needs %s >=%s,<%s, but %s %s is chosen%n",
                    r.who(), r.name(), r.lo(), r.hi(), r.name(), v);
            return null;
        }
        List<String> list = new ArrayList<>(VERSIONS.get(r.name()).keySet());
        list.sort(Comparator.comparing((String s) -> s, Main::cmp).reversed());
        for (String version : list) {
            if (!fits(version, r.lo(), r.hi())) {
                continue;
            }
            System.out.println("try " + r.name() + " " + version);
            List<Req> next = new ArrayList<>(rest);
            for (Req d : VERSIONS.get(r.name()).get(version)) {
                next.add(new Req(d.name(), d.lo(), d.hi(), r.name() + " " + version));
            }
            var c = new TreeMap<>(chosen);
            c.put(r.name(), version);
            var result = solve(next, c);
            if (result != null) {
                return result;
            }
        }
        return null;
    }

    public static void main(String[] args) {
        List<List<Req>> roots = List.of(
                List.of(new Req("web", "1.0", "3.0", "app"), new Req("log", "1.0", "2.0", "app")),
                List.of(new Req("web", "2.0", "3.0", "app"), new Req("log", "1.0", "2.0", "app")));
        for (List<Req> root : roots) {
            System.out.println("app needs " + root.stream()
                    .map(r -> r.name() + " >=" + r.lo() + ",<" + r.hi())
                    .collect(Collectors.joining(", ")));
            var result = solve(root, new TreeMap<>());
            if (result == null) {
                System.out.println("result: no solution");
            } else {
                System.out.println("result: " + result.entrySet().stream()
                        .map(e -> e.getKey() + " " + e.getValue())
                        .collect(Collectors.joining(", ")));
            }
        }
    }
}
```

```output
app needs web >=1.0,<3.0, log >=1.0,<2.0
try web 2.0
try log 1.2
  conflict: web 2.0 needs log >=2.0,<3.0, but log 1.2 is chosen
try log 1.1
  conflict: web 2.0 needs log >=2.0,<3.0, but log 1.1 is chosen
try log 1.0
  conflict: web 2.0 needs log >=2.0,<3.0, but log 1.0 is chosen
try web 1.1
try log 1.2
result: log 1.2, web 1.1
app needs web >=2.0,<3.0, log >=1.0,<2.0
try web 2.0
try log 1.2
  conflict: web 2.0 needs log >=2.0,<3.0, but log 1.2 is chosen
try log 1.1
  conflict: web 2.0 needs log >=2.0,<3.0, but log 1.1 is chosen
try log 1.0
  conflict: web 2.0 needs log >=2.0,<3.0, but log 1.0 is chosen
result: no solution
```

- Verdict: idiom. A record holds one requirement, and `Map.of` builds the registry.
- A `TreeMap` keeps the chosen versions sorted by name. A new `TreeMap` is made for each try, so a failed branch does not change the map of the caller.
- `null` means that the branch failed; an `Optional` would also work.

```java run id=tiny-package-manager
// A tiny package manager: install with dependencies, refuse to break them, and remove unused ones.
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

public class Main {
    static final Map<String, List<String>> REGISTRY = Map.of(
            "app", List.of("web", "log"),
            "web", List.of("http", "log"),
            "http", List.of(),
            "log", List.of(),
            "cli", List.of("log"));

    // A name maps to true if the user asked for it, false if it came as a dependency.
    static final TreeMap<String, Boolean> installed = new TreeMap<>();

    static String label(boolean asked) {
        return asked ? "(asked)" : "(dependency)";
    }

    static void install(String name, boolean asked) {
        if (installed.containsKey(name)) {
            installed.put(name, installed.get(name) || asked);
            return;
        }
        for (String dep : REGISTRY.get(name)) {
            install(dep, false);
        }
        installed.put(name, asked);
        System.out.println("  install " + name + " " + label(asked));
    }

    static List<String> neededBy(String name) {
        List<String> users = new ArrayList<>();
        for (String p : installed.keySet()) {
            if (REGISTRY.get(p).contains(name)) {
                users.add(p);
            }
        }
        return users;
    }

    static void remove(String name) {
        List<String> users = neededBy(name);
        if (!users.isEmpty()) {
            System.out.println("  error: " + name + " is needed by " + String.join(", ", users));
            return;
        }
        installed.remove(name);
        System.out.println("  remove " + name);
    }

    static void autoremove() {
        boolean changed = true;
        while (changed) {
            changed = false;
            for (String name : new ArrayList<>(installed.keySet())) {
                if (installed.containsKey(name) && !installed.get(name) && neededBy(name).isEmpty()) {
                    remove(name);
                    changed = true;
                }
            }
        }
    }

    public static void main(String[] args) {
        String[][] steps = {{"install", "app"}, {"install", "cli"}, {"remove", "log"},
                {"remove", "app"}, {"autoremove", ""}, {"list", ""}};
        for (String[] s : steps) {
            System.out.println(("$ " + s[0] + " " + s[1]).strip());
            switch (s[0]) {
                case "install" -> install(s[1], true);
                case "remove" -> remove(s[1]);
                case "autoremove" -> autoremove();
                default -> installed.forEach((name, asked) ->
                        System.out.println("  " + name + " " + label(asked)));
            }
        }
    }
}
```

```output
$ install app
  install http (dependency)
  install log (dependency)
  install web (dependency)
  install app (asked)
$ install cli
  install cli (asked)
$ remove log
  error: log is needed by app, cli, web
$ remove app
  remove app
$ autoremove
  remove web
  remove http
$ list
  cli (asked)
  log (dependency)
```

- Verdict: idiom. `Map.of` holds the registry, which does not change, and a `TreeMap` holds the installed packages in name order.
- `autoremove` loops over a copy of the keys, because `remove` changes the map during the loop.
- A switch with arrows runs one command for each step.

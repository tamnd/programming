```java run id=tiny-registry
// A registry keeps each published version for ever. A yank hides a version from new resolutions only.
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Optional;

public class Main {
    static final Comparator<String> BY_VERSION = Comparator.comparing(
            (String v) -> Arrays.stream(v.split("\\.")).mapToInt(Integer::parseInt).toArray(), Arrays::compare);

    static class Registry {
        final String name;
        final Map<String, Map<String, Boolean>> packages = new HashMap<>(); // package -> version -> yanked

        Registry(String name) {
            this.name = name;
        }

        void publish(String pkg, String version) {
            Map<String, Boolean> versions = packages.computeIfAbsent(pkg, k -> new HashMap<>());
            if (versions.containsKey(version)) {
                System.out.printf("publish %s: %s %s error: the version exists%n", name, pkg, version);
                return;
            }
            versions.put(version, false);
            System.out.printf("publish %s: %s %s ok%n", name, pkg, version);
        }

        void yank(String pkg, String version) {
            packages.get(pkg).put(version, true);
            System.out.printf("yank %s: %s %s%n", name, pkg, version);
        }

        void index(String pkg) {
            Map<String, Boolean> versions = packages.getOrDefault(pkg, Map.of());
            List<String> parts = new ArrayList<>();
            versions.keySet().stream().sorted(BY_VERSION)
                    .forEach(v -> parts.add(versions.get(v) ? v + " (yanked)" : v));
            System.out.printf("index of %s: %s%n", pkg, String.join(", ", parts));
        }

        // The newest version that is not yanked.
        Optional<String> find(String pkg) {
            return packages.getOrDefault(pkg, Map.of()).entrySet().stream()
                    .filter(e -> !e.getValue()).map(Map.Entry::getKey).max(BY_VERSION);
        }

        // An exact pin can still get a yanked version.
        Optional<String> find(String pkg, String pin) {
            return packages.getOrDefault(pkg, Map.of()).containsKey(pin) ? Optional.of(pin) : Optional.empty();
        }
    }

    static void bestMatch(List<Registry> registries, String pkg) {
        String version = null, from = null;
        for (Registry r : registries) {
            Optional<String> v = r.find(pkg);
            if (v.isPresent() && (version == null || BY_VERSION.compare(v.get(), version) > 0)) {
                version = v.get();
                from = r.name;
            }
        }
        System.out.printf("best match over all indexes: %s %s from %s%n", pkg, version, from);
    }

    static void firstIndex(List<Registry> registries, String pkg) {
        for (Registry r : registries) {
            Optional<String> v = r.find(pkg);
            if (v.isPresent()) {
                System.out.printf("first index with the package: %s %s from %s%n", pkg, v.get(), r.name);
                return;
            }
        }
    }

    public static void main(String[] args) {
        Registry pub = new Registry("public");
        pub.publish("log", "1.0.0");
        pub.publish("log", "1.1.0");
        pub.publish("log", "1.1.0");
        pub.yank("log", "1.1.0");
        pub.index("log");
        System.out.println("install log: " + pub.find("log").orElse("none"));
        System.out.println("install log ==1.1.0: " + pub.find("log", "1.1.0").orElse("none"));

        Registry internal = new Registry("internal");
        internal.publish("billing", "1.0.0");
        pub.publish("billing", "99.0.0"); // someone takes the same name on the public registry
        List<Registry> registries = List.of(internal, pub);
        bestMatch(registries, "billing");
        firstIndex(registries, "billing");
    }
}
```

```output
publish public: log 1.0.0 ok
publish public: log 1.1.0 ok
publish public: log 1.1.0 error: the version exists
yank public: log 1.1.0
index of log: 1.0.0, 1.1.0 (yanked)
install log: 1.0.0
install log ==1.1.0: 1.1.0
publish internal: billing 1.0.0 ok
publish public: billing 99.0.0 ok
best match over all indexes: billing 99.0.0 from public
first index with the package: billing 1.0.0 from internal
```

- Verdict: idiom. `Comparator.comparing` with `Arrays::compare` orders versions by their integer parts, and `Stream.max` returns an `Optional`.
- `computeIfAbsent` makes the inner map for a new package name, and `containsKey` refuses a second publish of the same version.
- Two overloads of `find` give the newest version that is not yanked, and the pinned version.

```java run id=rebuild
// Two rebuilders on one graph: file modification times (as in Make) and content hashes.
import java.nio.charset.StandardCharsets;
import java.security.MessageDigest;
import java.util.*;
import java.util.function.Consumer;

public class Main {
    static final Map<String, List<String>> RULES = Map.of(
        "main.o", List.of("main.c", "util.h"),
        "util.o", List.of("util.c", "util.h"),
        "app", List.of("main.o", "util.o"));
    static final List<String> ORDER = List.of("main.o", "util.o", "app");
    static final Map<String, String> SOURCES = Map.of(
        "main.c", "int main(void) { return util(); }",
        "util.c", "int util(void) { return 1; }",
        "util.h", "int util(void);");
    static int clock = 0;

    static int tick() {
        return ++clock;
    }

    // The compiler drops comments, so an object file depends only on the code.
    static String action(String target, Map<String, String> content) {
        List<String> parts = new ArrayList<>();
        for (String dep : RULES.get(target)) {
            parts.add(content.get(dep).split("//", 2)[0].stripTrailing());
        }
        return target + "(" + String.join(" | ", parts) + ")";
    }

    static String digest(String text) {
        try {
            byte[] hash = MessageDigest.getInstance("SHA-256").digest(text.getBytes(StandardCharsets.UTF_8));
            return HexFormat.of().formatHex(hash);
        } catch (Exception e) {
            throw new IllegalStateException(e);
        }
    }

    static String fromScratch(Map<String, String> content) {
        Map<String, String> values = new HashMap<>();
        for (String name : SOURCES.keySet()) {
            values.put(name, content.get(name));
        }
        for (String target : ORDER) {
            values.put(target, action(target, values));
        }
        return values.get("app");
    }

    static class Builder {
        final boolean byHash;
        final Map<String, String> content = new HashMap<>();
        final Map<String, Integer> mtime = new HashMap<>();
        // A target maps to the hashes of its dependencies at the last build.
        final Map<String, Map<String, String>> traces = new HashMap<>();

        Builder(boolean byHash) {
            this.byHash = byHash;
        }

        void write(String name, String text, int time) {
            content.put(name, text);
            mtime.put(name, time == 0 ? tick() : time);
        }

        boolean dirty(String target) {
            if (!content.containsKey(target)) {
                return true;
            }
            for (String dep : RULES.get(target)) {
                if (byHash && !traces.get(target).get(dep).equals(digest(content.get(dep)))) {
                    return true;
                }
                if (!byHash && mtime.get(dep) > mtime.get(target)) {
                    return true;
                }
            }
            return false;
        }

        String build() {
            List<String> ran = new ArrayList<>();
            for (String target : ORDER) {
                if (dirty(target)) {
                    write(target, action(target, content), 0);
                    Map<String, String> seen = new HashMap<>();
                    for (String dep : RULES.get(target)) {
                        seen.put(dep, digest(content.get(dep)));
                    }
                    traces.put(target, seen);
                    ran.add(target);
                }
            }
            return ran.isEmpty() ? "-" : String.join(" ", ran);
        }
    }

    record Step(String label, Consumer<Builder> edit) {}

    public static void main(String[] args) {
        List<Step> steps = List.of(
            new Step("clean build", b -> {
                for (String name : List.of("main.c", "util.c", "util.h")) {
                    b.write(name, SOURCES.get(name), 0);
                }
            }),
            new Step("touch util.h", b -> b.mtime.put("util.h", tick())),
            new Step("comment in main.c", b -> b.write("main.c", SOURCES.get("main.c") + " // entry point", 0)),
            // A copy from a backup keeps its old time, older than util.o.
            new Step("old util.c, old time", b -> b.write("util.c", "int util(void) { return 2; }", 1)));
        Builder byTime = new Builder(false);
        Builder byHash = new Builder(true);
        System.out.printf("%-24s%-20s%s%n", "step", "mtime", "hash");
        for (Step s : steps) {
            s.edit().accept(byTime);
            s.edit().accept(byHash);
            System.out.printf("%-24s%-20s%s%n", s.label(), byTime.build(), byHash.build());
        }
        System.out.println("mtime: app matches a clean build: " + byTime.content.get("app").equals(fromScratch(byTime.content)));
        System.out.println("hash: app matches a clean build: " + byHash.content.get("app").equals(fromScratch(byHash.content)));
    }
}
```

```output
step                    mtime               hash
clean build             main.o util.o app   main.o util.o app
touch util.h            main.o util.o app   -
comment in main.c       main.o app          main.o
old util.c, old time    -                   util.o app
mtime: app matches a clean build: false
hash: app matches a clean build: true
```

- Verdict: idiom. `MessageDigest.getInstance("SHA-256")` computes the hash, and `HexFormat.of().formatHex` turns the bytes into a string that `equals` can compare.
- `getInstance` declares a checked exception, so `digest` wraps it in an `IllegalStateException`.
- A `record Step` with a `Consumer<Builder>` holds each edit, and the loop applies it to both builders.

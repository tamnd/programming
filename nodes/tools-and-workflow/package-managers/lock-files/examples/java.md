```java run id=lock-file-with-hashes
// A lock file records the exact version and the SHA-256 hash of each package, and an install checks both.
import java.nio.charset.StandardCharsets;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.HexFormat;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;
import java.util.stream.Collectors;

public class Main {
    // name -> version -> the bytes of the package
    static final Map<String, Map<String, byte[]>> REGISTRY = new TreeMap<>();
    // name -> the major version that the project accepts
    static final Map<String, Integer> MANIFEST = new TreeMap<>(Map.of("log", 1, "web", 2));

    record Entry(String name, String version, String digest) {}

    static int[] key(String v) {
        return Arrays.stream(v.split("\\.")).mapToInt(Integer::parseInt).toArray();
    }

    static String sha256(byte[] data) throws NoSuchAlgorithmException {
        byte[] sum = MessageDigest.getInstance("SHA-256").digest(data);
        return HexFormat.of().formatHex(sum).substring(0, 16);
    }

    static void publish(String name, String version, String content) {
        REGISTRY.computeIfAbsent(name, n -> new TreeMap<>()).put(version, content.getBytes(StandardCharsets.UTF_8));
    }

    static List<Entry> resolve() {
        List<Entry> chosen = new ArrayList<>();
        for (var m : MANIFEST.entrySet()) {
            String best = REGISTRY.get(m.getKey()).keySet().stream()
                    .filter(v -> key(v)[0] == m.getValue())
                    .max(Comparator.comparing(Main::key, Arrays::compare))
                    .orElseThrow();
            chosen.add(new Entry(m.getKey(), best, ""));
        }
        System.out.println("resolve: " + chosen.stream()
                .map(e -> e.name() + " " + e.version()).collect(Collectors.joining(", ")));
        return chosen;
    }

    static List<Entry> lock(List<Entry> chosen) throws NoSuchAlgorithmException {
        System.out.println("lock file:");
        List<Entry> lockfile = new ArrayList<>();
        for (Entry e : chosen) {
            String digest = sha256(REGISTRY.get(e.name()).get(e.version()));
            System.out.println("  " + e.name() + " " + e.version() + " sha256 " + digest);
            lockfile.add(new Entry(e.name(), e.version(), digest));
        }
        return lockfile;
    }

    static void install(List<Entry> lockfile) throws NoSuchAlgorithmException {
        System.out.println("install from the lock file:");
        for (Entry e : lockfile) {
            String actual = sha256(REGISTRY.get(e.name()).get(e.version()));
            if (actual.equals(e.digest())) {
                System.out.println("  " + e.name() + " " + e.version() + " ok");
            } else {
                System.out.println("  " + e.name() + " " + e.version() + " error: expected sha256 "
                        + e.digest() + ", got " + actual);
            }
        }
    }

    public static void main(String[] args) throws NoSuchAlgorithmException {
        publish("log", "1.1.0", "log 1.1.0");
        publish("log", "1.2.0", "log 1.2.0");
        publish("web", "2.0.0", "web 2.0.0");
        publish("web", "2.0.1", "web 2.0.1");
        List<Entry> lockfile = lock(resolve());
        publish("log", "1.3.0", "log 1.3.0");
        System.out.println("log 1.3.0 is published");
        resolve();
        install(lockfile);
        publish("web", "2.0.1", "web 2.0.1 with other code");
        System.out.println("web 2.0.1 is changed on the registry");
        install(lockfile);
    }
}
```

```output
resolve: log 1.2.0, web 2.0.1
lock file:
  log 1.2.0 sha256 bc5900c6e46c5b3f
  web 2.0.1 sha256 a72ce45f235c38b5
log 1.3.0 is published
resolve: log 1.3.0, web 2.0.1
install from the lock file:
  log 1.2.0 ok
  web 2.0.1 ok
web 2.0.1 is changed on the registry
install from the lock file:
  log 1.2.0 ok
  web 2.0.1 error: expected sha256 a72ce45f235c38b5, got 6745f93439ad7399
```

- Verdict: idiom. `MessageDigest.getInstance("SHA-256")` computes the hash, and `HexFormat.of().formatHex` turns it into text.
- `getInstance` declares the checked exception `NoSuchAlgorithmException`, although every Java platform must support SHA-256.
- A `TreeMap` keeps the names in order, so the output is the same on each run.

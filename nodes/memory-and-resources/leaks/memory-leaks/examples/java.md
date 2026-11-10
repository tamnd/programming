```java run id=static-map
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.WeakHashMap;

public class Main {
    record Session(String user) {}

    // Leaks: a static map is reachable for the life of the class.
    static final Map<Integer, byte[]> RESULTS = new HashMap<>();

    // Fixed: a map in access order that drops the eldest entry above max.
    static final class Lru<K, V> extends LinkedHashMap<K, V> {
        private final int max;

        Lru(int max) {
            super(16, 0.75f, true);
            this.max = max;
        }

        @Override
        protected boolean removeEldestEntry(Map.Entry<K, V> eldest) {
            return size() > max;
        }
    }

    static final Map<Integer, byte[]> BOUNDED = new Lru<>(100);

    public static void main(String[] args) throws InterruptedException {
        for (int i = 0; i < 1000; i++) {
            RESULTS.put(i, new byte[1024]);
            BOUNDED.put(i, new byte[1024]);
        }
        System.out.println("static HashMap size: " + RESULTS.size());
        System.out.println("bounded LinkedHashMap size: " + BOUNDED.size());

        // Fixed: a WeakHashMap entry goes when its key is no longer in use.
        Map<Session, byte[]> perSession = new WeakHashMap<>();
        Session kept = new Session("kept");
        perSession.put(kept, new byte[1024]);
        for (int i = 0; i < 100; i++) {
            perSession.put(new Session("user-" + i), new byte[1024]);
        }
        long deadline = System.nanoTime() + 5_000_000_000L;
        while (perSession.size() > 1 && System.nanoTime() < deadline) {
            System.gc();
            Thread.sleep(10);
        }
        System.out.println("WeakHashMap size after gc: " + perSession.size());
        System.out.println("kept key still present: " + perSession.containsKey(kept));
    }
}
```

```output
static HashMap size: 1000
bounded LinkedHashMap size: 100
WeakHashMap size after gc: 1
kept key still present: true
```

- Verdict: native. The collector frees objects that no live thread can reach. A `static` field is reachable while its class is loaded, so a static map keeps each entry it holds.
- `LinkedHashMap` calls `removeEldestEntry` after each `put`. The `true` constructor argument selects access order, so the map drops the entry that the program used longest ago.
- `WeakHashMap` holds its keys weakly and its values strongly. A value that refers to one of the keys keeps that entry alive.
- The loop calls `System.gc()` until the map has one entry or 5 seconds pass, because a collection can run later than the call.

```java run id=guard
import java.util.HashMap;
import java.util.Map;

public class Main {
    static final Object lock = new Object();
    static final Map<String, Integer> table = new HashMap<>();

    static int increment(String path) {
        synchronized (lock) {  // released on every way out
            if (path.isEmpty()) return -1;
            table.merge(path, 1, Integer::sum);
            return 0;
        }
    }

    public static void main(String[] args) throws InterruptedException {
        Thread[] threads = new Thread[4];
        for (int i = 0; i < 4; i++) {
            threads[i] = new Thread(() -> { for (int j = 0; j < 1000; j++) increment("/index"); });
            threads[i].start();
        }
        for (Thread t : threads) t.join();
        System.out.println("hits /index: " + table.get("/index"));
        System.out.println("bad path returns " + increment(""));
        System.out.println("lock free after the early return: " + (Thread.holdsLock(lock) ? "False" : "True"));
        increment("/index");
        System.out.println("hits /index: " + table.get("/index"));
    }
}
```

```output
hits /index: 4000
bad path returns -1
lock free after the early return: True
hits /index: 4001
```

- Verdict: native. The `synchronized` block is the idiom in the language. `Thread.holdsLock` checks that the current thread no longer holds the monitor.

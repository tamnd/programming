```java run id=lazy
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.atomic.AtomicInteger;

public class Main {
    static final class Settings {
        private volatile Map<String, Integer> config; // volatile makes the check safe
        final AtomicInteger loads = new AtomicInteger();

        private Map<String, Integer> load() {
            System.out.println("loading config"); // stands for a slow read of a file
            loads.incrementAndGet();
            return Map.of("timeout", 30);
        }

        Map<String, Integer> config() {
            Map<String, Integer> result = config; // read the volatile field once
            if (result == null) {
                synchronized (this) {
                    result = config;
                    if (result == null) { // check again under the lock
                        config = result = load();
                    }
                }
            }
            return result;
        }
    }

    public static void main(String[] args) throws InterruptedException {
        Settings s = new Settings();
        System.out.println("settings created, loads = " + s.loads.get());
        System.out.println("timeout = " + s.config().get("timeout"));
        System.out.println("timeout = " + s.config().get("timeout"));
        System.out.println("loads = " + s.loads.get());

        Settings shared = new Settings();
        CountDownLatch start = new CountDownLatch(1);
        List<Thread> threads = new ArrayList<>();
        for (int i = 0; i < 8; i++) {
            Thread t = new Thread(() -> {
                try {
                    start.await(); // all eight threads ask at the same time
                } catch (InterruptedException e) {
                    return;
                }
                shared.config();
            });
            t.start();
            threads.add(t);
        }
        start.countDown();
        for (Thread t : threads) {
            t.join();
        }
        System.out.println("8 threads: loads = " + shared.loads.get());
    }
}
```

```output
settings created, loads = 0
loading config
timeout = 30
timeout = 30
loads = 1
loading config
8 threads: loads = 1
```

- Verdict: idiom. This is double-checked locking with a `volatile` field, the form of Effective Java Item 83 for an instance field. Without `volatile`, another thread can see the reference before it sees the fields of the object.
- For a static field, the lazy initialization holder class idiom is simpler. The JVM initializes a class only on first use, and class initialization is thread-safe, so a nested class with one static field needs no lock.
- `java.util.function.Supplier` is the usual type for a lazy value that a caller passes in.

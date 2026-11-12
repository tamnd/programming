```java run id=dcl
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.CyclicBarrier;
import java.util.concurrent.atomic.AtomicInteger;

public class Main {
    static final AtomicInteger created = new AtomicInteger();
    static final AtomicInteger lockedCalls = new AtomicInteger();

    static class Config {
        Config() { created.incrementAndGet(); }
    }

    private static volatile Config instance; // volatile: the JDK 5 fix
    private static final Object lock = new Object();

    static Config instance() {
        Config local = instance; // one volatile read on the fast path
        if (local == null) { // first check, no lock
            synchronized (lock) {
                lockedCalls.incrementAndGet();
                local = instance;
                if (local == null) { // second check, lock held
                    local = new Config();
                    instance = local;
                }
            }
        }
        return local;
    }

    public static void main(String[] args) throws InterruptedException {
        CyclicBarrier start = new CyclicBarrier(8);
        List<Thread> threads = new ArrayList<>();
        for (int w = 0; w < 8; w++) {
            Thread t = new Thread(() -> {
                try {
                    start.await();
                } catch (Exception e) {
                    throw new RuntimeException(e);
                }
                for (int i = 0; i < 1000; i++) instance();
            });
            threads.add(t);
            t.start();
        }
        for (Thread t : threads) t.join();
        int locked = lockedCalls.get();
        System.out.println("instances created: " + created.get());
        System.out.println("calls: " + 8 * 1000);
        System.out.println("calls that took the lock: " + (locked <= 8 ? "8 or fewer" : String.valueOf(locked)));
    }
}
```

```output
instances created: 1
calls: 8000
calls that took the lock: 8 or fewer
```

- Verdict: idiom. The field is `volatile`, and the local variable `local` reads it once on the fast path.

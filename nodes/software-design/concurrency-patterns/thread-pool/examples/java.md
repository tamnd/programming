```java run id=thread-pool
import java.util.ArrayList;
import java.util.List;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.atomic.AtomicInteger;

public class Main {
    static final AtomicInteger running = new AtomicInteger();
    static final AtomicInteger mostRunning = new AtomicInteger();
    static final Set<String> workers = ConcurrentHashMap.newKeySet();

    static long task(long n) throws InterruptedException {
        int now = running.incrementAndGet();
        mostRunning.accumulateAndGet(now, Math::max);
        workers.add(Thread.currentThread().getName());
        Thread.sleep(1);
        running.decrementAndGet();
        return n * n;
    }

    public static void main(String[] args) throws Exception {
        ExecutorService pool = Executors.newFixedThreadPool(4);
        List<Future<Long>> futures = new ArrayList<>();
        for (long n = 0; n < 100; n++) {
            final long k = n;
            futures.add(pool.submit(() -> task(k)));
        }
        long sum = 0;
        for (Future<Long> f : futures) {
            sum += f.get();
        }
        pool.shutdown();
        System.out.println("tasks run: " + futures.size());
        System.out.println("sum of results: " + sum);
        System.out.println("workers used, at most 4: " + (workers.size() <= 4 ? "True" : "False"));
        System.out.println("most tasks running at once, at most 4: " + (mostRunning.get() <= 4 ? "True" : "False"));
    }
}
```

```output
tasks run: 100
sum of results: 328350
workers used, at most 4: True
most tasks running at once, at most 4: True
```

- Verdict: stdlib. `Executors.newFixedThreadPool(4)` creates the pool, and `submit` returns a `Future` for each task.

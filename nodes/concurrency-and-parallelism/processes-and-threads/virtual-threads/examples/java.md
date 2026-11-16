```java run id=virtual-limit
import java.time.Duration;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Semaphore;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicLong;

public class Main {
    public static void main(String[] args) {
        final int tasks = 1_000;
        final Semaphore permits = new Semaphore(10);
        final AtomicInteger active = new AtomicInteger();
        final AtomicInteger peak = new AtomicInteger();
        final AtomicLong sum = new AtomicLong();
        try (ExecutorService executor = Executors.newVirtualThreadPerTaskExecutor()) {
            for (int i = 0; i < tasks; i++) {
                final int n = i;
                executor.submit(() -> {
                    permits.acquire();
                    try {
                        peak.accumulateAndGet(active.incrementAndGet(), Math::max);
                        Thread.sleep(Duration.ofMillis(10)); // the virtual thread unmounts here
                        sum.addAndGet(n);
                        active.decrementAndGet();
                    } finally {
                        permits.release();
                    }
                    return null;
                });
            }
        } // close() waits for every task
        System.out.println("tasks: " + tasks + ", sum: " + sum.get());
        System.out.println("never more than 10 at once: " + (peak.get() <= 10));
    }
}
```

```output for=virtual-limit
tasks: 1000, sum: 499500
never more than 10 at once: true
```

- Verdict: stdlib. Java 21 has virtual threads. `newVirtualThreadPerTaskExecutor` starts a new virtual thread for each task, and closing the executor waits for all of them.
- The executor creates 1,000 virtual threads, one for each task, and does not pool them. A `Semaphore` with 10 permits limits how many tasks wait at the same time.
- While a virtual thread sleeps, it unmounts, and its carrier runs other virtual threads.
- The `try` block with `finally` releases the permit even if the task fails.

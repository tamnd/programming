```java run id=sums
import java.time.Duration;
import java.util.Arrays;
import java.util.concurrent.Executors;
import java.util.concurrent.atomic.AtomicInteger;

public class Main {
    public static void main(String[] args) throws InterruptedException {
        long[] results = new long[4];
        Thread[] workers = new Thread[4];
        for (int i = 0; i < 4; i++) {
            int index = i;
            workers[i] = Thread.ofPlatform().start(() -> {
                long sum = 0;
                for (long n = index * 250L; n < (index + 1) * 250L; n++) {
                    sum += n;
                }
                results[index] = sum;
            });
        }
        for (Thread worker : workers) {
            worker.join();
        }
        System.out.println(Arrays.toString(results));
        System.out.println("platform: virtual=" + workers[0].isVirtual() + " daemon=" + workers[0].isDaemon());

        Thread virtual = Thread.ofVirtual().unstarted(() -> {});
        System.out.println("virtual: virtual=" + virtual.isVirtual() + " daemon=" + virtual.isDaemon());

        AtomicInteger done = new AtomicInteger();
        try (var executor = Executors.newVirtualThreadPerTaskExecutor()) {
            for (int i = 0; i < 10_000; i++) {
                executor.submit(() -> {
                    Thread.sleep(Duration.ofMillis(10));
                    return done.incrementAndGet();
                });
            }
        }
        System.out.println("virtual threads finished: " + done.get());
    }
}
```

```output for=sums
[31125, 93625, 156125, 218625]
platform: virtual=false daemon=false
virtual: virtual=true daemon=true
virtual threads finished: 10000
```

- Verdict: stdlib. `java.lang.Thread` makes platform threads, which wrap OS threads, and virtual threads, which the JDK runs on a few carrier threads (M:N).
- `join()` waits for the thread. A `Runnable` returns nothing, so each thread writes its own array element.
- A virtual thread is always a daemon thread. Closing the executor waits for all 10,000 tasks.
- A sleeping virtual thread frees its carrier for other tasks.

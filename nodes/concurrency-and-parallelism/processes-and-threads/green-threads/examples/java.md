```java run id=virtual-sleep
import java.time.Duration;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

public class Main {
    public static void main(String[] args) throws InterruptedException, ExecutionException {
        final int count = 10_000;
        long start = System.nanoTime();
        long sum = 0;
        try (ExecutorService executor = Executors.newVirtualThreadPerTaskExecutor()) {
            List<Future<Integer>> futures = new ArrayList<>();
            for (int i = 0; i < count; i++) {
                final int n = i;
                futures.add(executor.submit(() -> {
                    Thread.sleep(Duration.ofMillis(100)); // unmounts the virtual thread
                    return n;
                }));
            }
            for (Future<Integer> future : futures) {
                sum += future.get();
            }
        }
        Duration elapsed = Duration.ofNanos(System.nanoTime() - start);
        System.out.println("virtual threads: " + count + ", sum: " + sum);
        System.out.println("all slept 100 ms in under 5 s: " + (elapsed.compareTo(Duration.ofSeconds(5)) < 0));
    }
}
```

```output for=virtual-sleep
virtual threads: 10000, sum: 49995000
all slept 100 ms in under 5 s: true
```

- Verdict: stdlib. Java 1.1 had green threads, and later versions dropped them. Java 21 has virtual threads, which the JDK schedules M:N onto a small pool of platform threads.
- `newVirtualThreadPerTaskExecutor` starts a new virtual thread for each task. The `try` block closes the executor, and the close waits for all tasks.
- `Thread.sleep` on a virtual thread does not block its carrier thread, so 10,000 sleeps of 100 ms overlap.
- See [[virtual-threads]] for pinning and the other limits of the model.

```java run id=executor-policies
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.Executor;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class Main {
    // This code knows only the Executor interface. It returns the IDs of the threads that ran the tasks.
    static Set<Long> threadsUsed(Executor executor, int count) throws InterruptedException {
        Set<Long> threads = ConcurrentHashMap.newKeySet();
        CountDownLatch done = new CountDownLatch(count);
        for (int i = 0; i < count; i++) {
            executor.execute(() -> {
                threads.add(Thread.currentThread().threadId());
                done.countDown();
            });
        }
        done.await();
        return threads;
    }

    public static void main(String[] args) throws InterruptedException {
        long caller = Thread.currentThread().threadId();

        Executor direct = Runnable::run;
        System.out.println("direct: ran on the caller's thread only: "
            + threadsUsed(direct, 4).equals(Set.of(caller)));

        try (ExecutorService perTask = Executors.newThreadPerTaskExecutor(Thread.ofPlatform().factory())) {
            System.out.println("thread per task: threads used: " + threadsUsed(perTask, 4).size());
        }

        try (ExecutorService pool = Executors.newFixedThreadPool(2)) {
            System.out.println("pool of 2: at most 2 threads: " + (threadsUsed(pool, 4).size() <= 2));
        }
    }
}
```

```output for=executor-policies
direct: ran on the caller's thread only: true
thread per task: threads used: 4
pool of 2: at most 2 threads: true
```

- Verdict: stdlib. `java.util.concurrent.Executor` has one method, `execute(Runnable)`. `ExecutorService` adds `submit`, futures and shutdown, and `Executors` has factory methods for common executors.
- `threadsUsed` does not know how the tasks run. The same method runs them in the caller's thread, on a new thread each, and on a pool of two threads.
- `Runnable::run` is the direct executor of the `Executor` documentation: it runs each task in the caller's thread.
- A `CountDownLatch` waits for the tasks, because the `Executor` interface gives no way to wait.

```java run id=per-thread-values
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class Main {
    // Each thread gets its own counter, which starts at 0.
    static final ThreadLocal<Integer> COUNTER = ThreadLocal.withInitial(() -> 0);
    static final ThreadLocal<String> USER = new ThreadLocal<>();

    static int bump(int times) {
        for (int i = 0; i < times; i++) COUNTER.set(COUNTER.get() + 1);
        return COUNTER.get();
    }

    public static void main(String[] args) throws InterruptedException, ExecutionException {
        Map<String, Integer> results = new ConcurrentHashMap<>();
        Thread t1 = new Thread(() -> results.put("thread 1", bump(1)));
        Thread t2 = new Thread(() -> results.put("thread 2", bump(2)));
        t1.start();
        t2.start();
        t1.join();
        t2.join();
        results.put("main", bump(3));
        for (String name : new String[] {"main", "thread 1", "thread 2"}) {
            System.out.println(name + ": counter = " + results.get(name));
        }

        // A pool reuses its threads, so a value that one task leaves behind is seen by the next task.
        try (ExecutorService pool = Executors.newSingleThreadExecutor()) {
            pool.submit(() -> USER.set("alice")).get();
            System.out.println("a later task on the same thread sees user: " + pool.submit(USER::get).get());
            pool.submit(USER::remove).get();
            System.out.println("after a task removes it, the next task sees user: " + pool.submit(USER::get).get());
        }
    }
}
```

```output for=per-thread-values
main: counter = 3
thread 1: counter = 1
thread 2: counter = 2
a later task on the same thread sees user: alice
after a task removes it, the next task sees user: null
```

- Verdict: stdlib. `ThreadLocal` gives each thread its own, independently initialized copy. `withInitial` sets the starting value for each thread.
- A single-thread executor runs all its tasks on one thread. The value that the first task sets stays until a task calls `remove`.
- In a pool, call `remove` when a task ends, usually in a `finally` block, so that the next task does not see the value.
- `InheritableThreadLocal` copies values from a parent thread to its child threads. Scoped values are a newer API for data that flows one way, from a caller to the methods that it calls.

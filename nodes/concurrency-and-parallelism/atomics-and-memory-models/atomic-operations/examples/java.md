```java run id=atomic
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.LongAdder;

public class Main {
    static volatile boolean ready;
    static String message;

    public static void main(String[] args) throws Exception {
        AtomicInteger hits = new AtomicInteger();
        LongAdder visits = new LongAdder();
        ExecutorService pool = Executors.newFixedThreadPool(4);
        for (int t = 0; t < 4; t++) {
            pool.submit(() -> {
                for (int i = 0; i < 100_000; i++) {
                    hits.incrementAndGet();
                    visits.increment();
                }
            });
        }
        pool.shutdown();
        pool.awaitTermination(1, TimeUnit.MINUTES);
        System.out.println(hits.get() + " " + visits.sum());

        System.out.println(hits.compareAndSet(400_000, 0) + " " + hits.get());

        Thread producer = new Thread(() -> {
            message = "hello";
            ready = true;
        });
        producer.start();
        while (!ready) {
            Thread.onSpinWait();
        }
        System.out.println(message);
    }
}
```

```output for=atomic
400000 400000
true 0
hello
```

- Verdict: stdlib. `java.util.concurrent.atomic` has `AtomicInteger`, `AtomicLong`, `AtomicReference` and the adders. `VarHandle` gives atomic access with explicit memory order to fields and array elements.
- A write to a `volatile` field happens-before every later read of that field. So the write to `message` before `ready = true` is visible to the thread that sees `ready`.
- `LongAdder` spreads updates over several cells, so it is faster than one `AtomicLong` when many threads add at the same time.

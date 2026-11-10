```java run id=atomic
import java.util.concurrent.atomic.AtomicInteger;

public class Main {
    public static void main(String[] args) throws InterruptedException {
        AtomicInteger count = new AtomicInteger();
        Runnable work = () -> {
            for (int i = 0; i < 100_000; i++) {
                count.incrementAndGet();
            }
        };
        Thread first = new Thread(work);
        Thread second = new Thread(work);
        first.start();
        second.start();
        first.join();
        second.join();
        System.out.println(count.get());
    }
}
```

```output for=atomic
200000
```

- Verdict: stdlib. `AtomicInteger.incrementAndGet` is one atomic read-modify-write, so the two threads never lose an update.
- A data race in Java does not break type safety. A racing read sees some written value, except that a plain `long` or `double` can tear into two 32-bit halves.
- A `volatile` field also removes the race on a single read or write. It does not make `count++` atomic, because that is still a read and a separate write.
- `Thread.join` orders the writes of each thread before the final `count.get()`.

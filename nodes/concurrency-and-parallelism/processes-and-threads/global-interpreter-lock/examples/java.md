```java run id=threads-and-the-gil
import java.util.concurrent.atomic.AtomicLong;

public class Main {
  public static void main(String[] args) throws InterruptedException {
    // The JVM has no global interpreter lock: these threads can run on several cores at once.
    // So a shared counter must be atomic, or guarded by a lock.
    AtomicLong total = new AtomicLong();
    Thread[] threads = new Thread[4];
    for (int t = 0; t < threads.length; t++) {
      threads[t] =
          Thread.ofPlatform()
              .start(
                  () -> {
                    for (int i = 0; i < 100_000; i++) {
                      total.incrementAndGet();
                    }
                  });
    }
    for (Thread t : threads) {
      t.join();
    }
    System.out.println("4 threads added 100000 each with AtomicLong");
    System.out.println("total: " + total.get());
  }
}
```

```output for=threads-and-the-gil
4 threads added 100000 each with AtomicLong
total: 400000
```

- Verdict: absent. The JVM has no GIL. The Java Language Specification says that threads may run on "many hardware processors".
- So two threads can change a shared counter at the same moment. An `AtomicLong` is "A long value that may be updated atomically", so each `incrementAndGet` is one atomic step.

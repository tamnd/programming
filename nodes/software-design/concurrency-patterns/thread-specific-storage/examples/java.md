```java run id=tss
import java.util.concurrent.atomic.AtomicInteger;

public class Main {
    // One access point, one value per thread.
    static final ThreadLocal<Integer> lastError = ThreadLocal.withInitial(() -> 0);

    static int fakeCall(int code) {
        lastError.set(code); // the call sets its thread's error
        Thread.yield(); // let another thread run between the set and the test
        return -1;
    }

    public static void main(String[] args) throws InterruptedException {
        AtomicInteger wrongReads = new AtomicInteger();
        int[] seen = new int[4];
        Thread[] threads = new Thread[4];
        for (int i = 0; i < 4; i++) {
            final int index = i;
            threads[i] = new Thread(() -> {
                int code = index + 1;
                for (int n = 0; n < 1000; n++) {
                    if (fakeCall(code) == -1 && lastError.get() != code) wrongReads.incrementAndGet();
                }
                seen[index] = lastError.get();
            });
            threads[i].start();
        }
        for (Thread t : threads) t.join();
        System.out.println("threads: " + threads.length);
        System.out.println("calls per thread: " + 1000);
        System.out.println("reads that saw another thread's error: " + wrongReads.get());
        System.out.println("error code seen by each thread: [" + seen[0] + ", " + seen[1] + ", " + seen[2] + ", " + seen[3] + "]");
    }
}
```

```output
threads: 4
calls per thread: 1000
reads that saw another thread's error: 0
error code seen by each thread: [1, 2, 3, 4]
```

- Verdict: stdlib. `ThreadLocal` is the TS Object Proxy: `get` and `set` reach the value of the calling thread.

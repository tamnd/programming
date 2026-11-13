```java run id=barrier-pattern
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.concurrent.CyclicBarrier;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;

public class Main {
    static final int N = 8;
    static final int[] values = new int[N];
    static final int[] distances = {1, 2, 4}; // each step adds the value that is d places to the left

    public static void main(String[] args) throws Exception {
        for (int i = 0; i < N; i++) values[i] = i + 1;
        System.out.println("input: " + Arrays.toString(values));
        AtomicInteger trips = new AtomicInteger();
        // The action runs once per trip, before the threads are released.
        CyclicBarrier barrier = new CyclicBarrier(N, trips::incrementAndGet);

        List<Thread> threads = new ArrayList<>();
        for (int w = 0; w < N; w++) {
            int i = w;
            threads.add(new Thread(() -> {
                try {
                    for (int d : distances) {
                        // Phase 1: read a value that the last step wrote.
                        int add = i >= d ? values[i - d] : 0;
                        barrier.await(5, TimeUnit.SECONDS); // nobody writes until every worker has read
                        // Phase 2: write the new value.
                        values[i] += add;
                        barrier.await(5, TimeUnit.SECONDS); // nobody reads until every worker has written
                    }
                } catch (Exception e) {
                    throw new RuntimeException(e);
                }
            }));
        }
        for (Thread t : threads) t.start();
        for (Thread t : threads) t.join();

        boolean matches = true;
        for (int i = 0; i < N; i++) matches &= values[i] == (i + 1) * (i + 2) / 2;
        System.out.println("steps: " + distances.length);
        System.out.println("barrier trips: " + trips.get());
        System.out.println("prefix sums: " + Arrays.toString(values));
        System.out.println("matches a running total: " + (matches ? "True" : "False"));
    }
}
```

```output
input: [1, 2, 3, 4, 5, 6, 7, 8]
steps: 3
barrier trips: 6
prefix sums: [1, 3, 6, 10, 15, 21, 28, 36]
matches a running total: True
```

- Verdict: stdlib. `CyclicBarrier(parties, barrierAction)` with `await`; `Phaser` is the choice when the number of parties changes.

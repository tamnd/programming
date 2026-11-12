```java run id=copy-on-write
import java.util.ArrayList;
import java.util.ConcurrentModificationException;
import java.util.List;
import java.util.concurrent.CopyOnWriteArrayList;
import java.util.concurrent.atomic.AtomicInteger;

public class Main {
    public static void main(String[] args) throws InterruptedException {
        CopyOnWriteArrayList<String> listeners = new CopyOnWriteArrayList<>(List.of("listener 0"));
        List<String> first = new ArrayList<>();
        for (String name : listeners) first.add(name); // iterate over the first snapshot
        AtomicInteger iterations = new AtomicInteger();
        AtomicInteger errors = new AtomicInteger();
        List<Thread> threads = new ArrayList<>();
        for (int i = 0; i < 4; i++) {
            threads.add(new Thread(() -> {
                for (int r = 0; r < 1000; r++) {
                    try {
                        for (String name : listeners) {
                            // call each listener
                        }
                    } catch (ConcurrentModificationException e) {
                        errors.incrementAndGet();
                    }
                    iterations.incrementAndGet();
                }
            }));
        }
        threads.add(new Thread(() -> {
            for (int n = 1; n <= 100; n++) listeners.add("listener " + n); // copies the array
        }));
        for (Thread t : threads) t.start();
        for (Thread t : threads) t.join();
        System.out.println("iterations: " + iterations.get());
        System.out.println("iterations that failed: " + errors.get());
        System.out.println("listeners at the end: " + listeners.size());
        System.out.println("length of the first snapshot: " + first.size());
    }
}
```

```output
iterations: 4000
iterations that failed: 0
listeners at the end: 101
length of the first snapshot: 1
```

- Verdict: stdlib. `CopyOnWriteArrayList` copies its array on each `add`, and each `for` loop iterates over the snapshot from the moment the loop started.

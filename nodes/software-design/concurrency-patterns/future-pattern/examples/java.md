```java run id=future-pattern
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.concurrent.CyclicBarrier;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.TimeUnit;

public class Main {
    public static void main(String[] args) throws Exception {
        String[] items = {"apple", "bread", "cheese"};
        Map<String, Integer> prices = Map.of("apple", 12, "bread", 7, "cheese", 30);
        // The lookups wait here with the caller, so the output does not depend on timing.
        CyclicBarrier meeting = new CyclicBarrier(4);

        ExecutorService pool = Executors.newFixedThreadPool(3);
        // Each call returns a future at once. The lookup runs on a pool thread.
        List<Future<Integer>> futures = new ArrayList<>();
        for (String item : items) {
            futures.add(pool.submit(() -> {
                meeting.await(5, TimeUnit.SECONDS); // returns when all 3 lookups and the caller are here
                return prices.get(item);
            }));
        }
        System.out.println("lookups started: " + futures.size());
        boolean anyDone = futures.stream().anyMatch(Future::isDone);
        System.out.println("done right after the calls: " + (anyDone ? "True" : "False"));
        meeting.await(5, TimeUnit.SECONDS); // the caller is free to do other work here
        System.out.println("all three lookups ran at once: " + (meeting.isBroken() ? "False" : "True"));
        List<Integer> results = new ArrayList<>();
        for (Future<Integer> f : futures) results.add(f.get()); // wait for each result
        pool.shutdown();

        System.out.println("prices: " + results);
        System.out.println("total: " + results.stream().mapToInt(Integer::intValue).sum());
    }
}
```

```output
lookups started: 3
done right after the calls: False
all three lookups ran at once: True
prices: [12, 7, 30]
total: 49
```

- Verdict: stdlib. `ExecutorService.submit` returns a `Future<Integer>` at once; `isDone()` checks it and `get()` waits for the value.

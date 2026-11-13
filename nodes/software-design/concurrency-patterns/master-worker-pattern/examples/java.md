```java run id=master-worker-pattern
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.Callable;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

public class Main {
    static final int LIMIT = 20000;
    static final int WORKERS = 4;

    // The work of one worker: count the primes in [lo, hi).
    static int countPrimes(int lo, int hi) {
        int count = 0;
        for (int n = Math.max(lo, 2); n < hi; n++) {
            int d = 2;
            while (d * d <= n && n % d != 0) d++;
            if (d * d > n) count++;
        }
        return count;
    }

    public static void main(String[] args) throws Exception {
        // 1. Split the work into one part for each worker.
        List<int[]> parts = new ArrayList<>();
        for (int i = 0; i < WORKERS; i++) parts.add(new int[] {LIMIT * i / WORKERS, LIMIT * (i + 1) / WORKERS});
        List<Callable<Integer>> tasks = new ArrayList<>();
        for (int[] p : parts) tasks.add(() -> countPrimes(p[0], p[1]));

        // 2. Run the parts on the workers; invokeAll returns when all are done.
        List<Integer> results = new ArrayList<>();
        ExecutorService workers = Executors.newFixedThreadPool(WORKERS);
        try {
            for (Future<Integer> f : workers.invokeAll(tasks)) results.add(f.get());
        } finally {
            workers.shutdown();
        }
        // 3. Combine the partial results.
        int total = results.stream().mapToInt(Integer::intValue).sum();

        List<String> shown = new ArrayList<>();
        for (int[] p : parts) shown.add("(" + p[0] + ", " + p[1] + ")");
        System.out.println("parts: [" + String.join(", ", shown) + "]");
        System.out.println("primes in each part: " + results);
        System.out.println("primes below " + LIMIT + " from the master: " + total);
        System.out.println("same as one loop: " + (total == countPrimes(0, LIMIT) ? "True" : "False"));
    }
}
```

```output
parts: [(0, 5000), (5000, 10000), (10000, 15000), (15000, 20000)]
primes in each part: [669, 560, 525, 508]
primes below 20000 from the master: 2262
same as one loop: True
```

- Verdict: stdlib. `Executors.newFixedThreadPool` and `invokeAll`, which returns the futures when all the parts are done.

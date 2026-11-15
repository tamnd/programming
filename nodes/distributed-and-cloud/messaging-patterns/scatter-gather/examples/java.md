```java run id=best-quote
import java.util.Map;
import java.util.TreeMap;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.TimeUnit;

public class Main {
    record Offer(long delayMs, int price) {}

    static int ask(Offer o) {
        try {
            Thread.sleep(o.delayMs()); // a stand-in for a request on the channel of the supplier
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
        return o.price();
    }

    public static void main(String[] args) {
        // Each supplier replies after some time with a price in cents.
        Map<String, Offer> suppliers = new TreeMap<>(Map.of(
            "alpha", new Offer(20, 1250), "beta", new Offer(80, 1190), "gamma", new Offer(1000, 1100)));
        System.out.printf("asked %d suppliers, wait at most 400 ms%n", suppliers.size());

        Map<String, CompletableFuture<Integer>> futures = new TreeMap<>();
        suppliers.forEach((name, offer) -> futures.put(name,
            CompletableFuture.supplyAsync(() -> ask(offer))        // scatter
                .completeOnTimeout(null, 400, TimeUnit.MILLISECONDS))); // null means no reply in time

        String best = null;
        int bestPrice = Integer.MAX_VALUE;
        for (var e : futures.entrySet()) {                         // gather
            Integer price = e.getValue().join();
            if (price == null) {
                System.out.printf("no reply from %s in time%n", e.getKey());
                continue;
            }
            System.out.printf("reply from %s: %d%n", e.getKey(), price);
            if (price < bestPrice) {
                best = e.getKey();
                bestPrice = price;
            }
        }
        System.out.printf("best: %s at %d%n", best, bestPrice);
    }
}
```

```output
asked 3 suppliers, wait at most 400 ms
reply from alpha: 1250
reply from beta: 1190
no reply from gamma in time
best: beta at 1190
```

- Verdict: idiom. `CompletableFuture.supplyAsync` scatters the requests, and `completeOnTimeout(null, 400, TimeUnit.MILLISECONDS)` gives each future a value when the time is over.
- `join` on each future then waits at most about 400 ms in total, because all the timers start at the same time.
- The common pool has daemon threads, so the program ends while gamma still sleeps.

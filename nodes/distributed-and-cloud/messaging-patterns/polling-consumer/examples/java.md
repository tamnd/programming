```java run id=poll-orders
import java.util.*;
import java.util.concurrent.*;

public class Main {
    static final BlockingQueue<String> orders = new LinkedBlockingQueue<>();

    // Wait up to timeoutMs for one message, then take up to max messages.
    static List<String> poll(int max, long timeoutMs) throws InterruptedException {
        String first = timeoutMs == 0 ? orders.poll() : orders.poll(timeoutMs, TimeUnit.MILLISECONDS);
        List<String> batch = new ArrayList<>();
        if (first == null) return batch;
        batch.add(first);
        orders.drainTo(batch, max - 1);
        return batch;
    }

    public static void main(String[] args) throws InterruptedException {
        System.out.println("poll 0, no wait: " + (poll(2, 0).isEmpty() ? "empty" : "?"));
        for (int n = 1; n <= 5; n++) orders.add("order-" + n);
        System.out.println("producer: 5 orders on the queue");

        ScheduledExecutorService producer = Executors.newSingleThreadScheduledExecutor();
        for (int n = 1; ; n++) {
            if (n == 4) {
                // a producer sends one more order while the consumer waits
                producer.schedule(() -> orders.add("order-6"), 50, TimeUnit.MILLISECONDS);
            }
            List<String> batch = poll(2, 300);
            if (batch.isEmpty()) {
                System.out.println("poll " + n + ": nothing in 300 ms, the consumer stops");
                break;
            }
            System.out.println("poll " + n + ": " + String.join(" ", batch) + " (" + orders.size() + " left)");
            // the consumer works on the batch here, at its own pace
        }
        producer.shutdown();
    }
}
```

```output
poll 0, no wait: empty
producer: 5 orders on the queue
poll 1: order-1 order-2 (3 left)
poll 2: order-3 order-4 (1 left)
poll 3: order-5 (0 left)
poll 4: order-6 (0 left)
poll 5: nothing in 300 ms, the consumer stops
```

- Verdict: idiom. `BlockingQueue.poll()` returns `null` at once, and `poll(timeout, unit)` waits up to the timeout; this is the same pair as `receiveNoWait()` and `receive(long)` in a messaging API.
- `drainTo(batch, max - 1)` takes the rest of the batch with no wait.
- A `ScheduledExecutorService` plays a producer that sends while the consumer waits in poll 4.

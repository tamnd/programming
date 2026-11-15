```java run id=three-subscribers
import java.util.ArrayList;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;

public class Main {
    static final String STOP = "stop";

    // A publish-subscribe channel: one queue for each subscriber.
    static final class Topic {
        private final Map<String, BlockingQueue<String>> queues = new LinkedHashMap<>();

        BlockingQueue<String> subscribe(String name) {
            BlockingQueue<String> q = new LinkedBlockingQueue<>();
            queues.put(name, q);
            return q;
        }

        int publish(String event) {
            for (BlockingQueue<String> q : queues.values()) {
                q.add(event); // each subscriber gets its own copy
            }
            return queues.size();
        }
    }

    public static void main(String[] args) throws InterruptedException {
        Topic topic = new Topic();
        Map<String, List<String>> seen = new LinkedHashMap<>();
        List<Thread> threads = new ArrayList<>();
        for (String name : List.of("billing", "email", "stock")) {
            BlockingQueue<String> inbox = topic.subscribe(name);
            List<String> got = new ArrayList<>();
            seen.put(name, got);
            Thread t = new Thread(() -> {
                try {
                    for (String e = inbox.take(); !e.equals(STOP); e = inbox.take()) {
                        got.add(e);
                    }
                } catch (InterruptedException ex) {
                    Thread.currentThread().interrupt();
                }
            });
            threads.add(t);
            t.start();
        }

        int copies = 0;
        for (int n = 1; n <= 3; n++) {
            copies += topic.publish("order " + n + " placed");
        }
        topic.publish(STOP);
        for (Thread t : threads) {
            t.join(); // join makes the list of each thread visible here
        }

        seen.forEach((name, events) -> System.out.println(name + ": " + String.join(", ", events)));
        System.out.println("copies sent: " + copies);
    }
}
```

```output
billing: order 1 placed, order 2 placed, order 3 placed
email: order 1 placed, order 2 placed, order 3 placed
stock: order 1 placed, order 2 placed, order 3 placed
copies sent: 9
```

- Verdict: stdlib. `Topic` keeps one `LinkedBlockingQueue` for each subscriber, and `publish` adds the event to each.
- Each thread fills its own list, and `join` makes the list visible to the main thread, so the lists need no lock.
- With Jakarta Messaging, the program creates a consumer on a `Topic` for each subscriber, and the provider makes the copies. A durable subscription needs a name, for example with `createDurableConsumer`.

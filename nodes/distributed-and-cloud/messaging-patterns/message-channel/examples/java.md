```java run id=named-channels
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.TimeUnit;

public class Main {
    // Named channels. Senders and receivers share only the names.
    static final class Channels {
        private final Map<String, BlockingQueue<String>> byName = new LinkedHashMap<>();

        Channels(String... names) {
            for (String n : names) {
                byName.put(n, new LinkedBlockingQueue<>());
            }
        }

        void send(String name, String message) {
            BlockingQueue<String> q = byName.get(name);
            if (q == null) {
                throw new IllegalArgumentException("no channel named " + name);
            }
            q.add(message);
        }

        String receive(String name, long millis) throws InterruptedException {
            return byName.get(name).poll(millis, TimeUnit.MILLISECONDS);
        }

        int depth(String name) {
            return byName.get(name).size();
        }
    }

    public static void main(String[] args) throws InterruptedException {
        Channels channels = new Channels("orders", "payments");
        channels.send("orders", "order 1: 2 x A-7");
        channels.send("orders", "order 2: 1 x B-1");
        channels.send("payments", "payment for order 1: 30.00");
        System.out.printf("waiting: orders=%d payments=%d%n", channels.depth("orders"), channels.depth("payments"));

        String[][] receivers = {{"orders", "warehouse"}, {"payments", "accounting"}};
        for (String[] r : receivers) {
            Thread t = new Thread(() -> {
                try {
                    String m;
                    while ((m = channels.receive(r[0], 100)) != null) {
                        System.out.println(r[1] + ": " + m);
                    }
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
            });
            t.start();
            t.join();
        }

        try {
            channels.send("order", "order 3: 1 x C-2");
        } catch (IllegalArgumentException e) {
            System.out.println("error: " + e.getMessage());
        }
    }
}
```

```output
waiting: orders=2 payments=1
warehouse: order 1: 2 x A-7
warehouse: order 2: 1 x B-1
accounting: payment for order 1: 30.00
error: no channel named order
```

- Verdict: stdlib. `LinkedBlockingQueue` is a thread-safe queue, and `poll` with a timeout lets a receiver stop when its channel is empty.
- `send` throws `IllegalArgumentException` for an unknown name. In Jakarta Messaging, a client gets a `Queue` or `Topic` object for the name, and then a producer or consumer for it.
- The map is a `LinkedHashMap` only so that the order of the channels is stable. The receivers do not depend on it.

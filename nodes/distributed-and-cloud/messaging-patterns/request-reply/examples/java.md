```java run id=price-requests
import java.util.Map;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.TimeUnit;

public class Main {
    record Message(String id, String replyTo, String correlationId, String body) {}

    static final Map<String, Integer> PRICES = Map.of("A1", 1999, "B2", 500);
    static final Message STOP = new Message("", "", "", "");

    // Named channels, as on a broker: one request channel and one reply channel for each requestor.
    static final Map<String, BlockingQueue<Message>> CHANNELS = Map.of(
        "prices", new LinkedBlockingQueue<>(),
        "replies.web", new LinkedBlockingQueue<>(),
        "replies.mobile", new LinkedBlockingQueue<>());

    static void replier() {
        try {
            Message request;
            while ((request = CHANNELS.get("prices").take()) != STOP) {
                var price = PRICES.get(request.body());
                var body = price != null ? price + " cents" : "error unknown sku " + request.body();
                CHANNELS.get(request.replyTo()).put(new Message("", "", request.id(), body)); // the return address
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }

    static String ask(String requestor, String id, String sku) throws InterruptedException {
        var replyTo = "replies." + requestor;
        CHANNELS.get("prices").put(new Message(id, replyTo, "", sku));
        var reply = CHANNELS.get(replyTo).poll(2, TimeUnit.SECONDS); // block, but not for ever
        if (reply == null) {
            return requestor + " asked for " + sku + ": no reply in 2 s";
        }
        return requestor + " asked for " + sku + ": " + reply.body();
    }

    public static void main(String[] args) throws InterruptedException {
        var worker = new Thread(Main::replier);
        worker.start();
        System.out.println(ask("web", "q1", "A1"));
        System.out.println(ask("mobile", "q2", "B2"));
        System.out.println(ask("web", "q3", "Z9"));
        CHANNELS.get("prices").put(STOP); // stop the replier
        worker.join();
    }
}
```

```output
web asked for A1: 1999 cents
mobile asked for B2: 500 cents
web asked for Z9: error unknown sku Z9
```

- Verdict: idiom. Java has no request-reply channel in the standard library. A `LinkedBlockingQueue` for each channel and a thread for the replier give the pattern in one process.
- `poll(2, TimeUnit.SECONDS)` returns `null` when no reply comes in 2 seconds, so the requestor can report a timeout.
- The message `STOP` is compared by reference with `!=`, so it cannot be mistaken for a real request with the same fields.

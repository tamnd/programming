```java run id=match-replies
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class Main {
    record Request(String id, String sku) {}

    record Reply(String correlationId, String body) {}

    static final Map<String, Integer> PRICES = Map.of("A1", 1999, "B2", 500);

    public static void main(String[] args) {
        // One reply channel for all the requests of this requestor.
        var replyChannel = new ArrayList<Reply>();
        replyChannel.add(new Reply("q0", "1999 cents")); // left from a request that timed out

        var pending = new HashMap<String, String>(); // request id -> what the request asked for
        var requests = List.of(new Request("q1", "A1"), new Request("q2", "B2"), new Request("q3", "Z9"));
        for (var r : requests) {
            pending.put(r.id(), r.sku());
        }

        // The replier works on the requests in a different order, and it copies the id of each request.
        for (var r : requests.reversed()) {
            var price = PRICES.get(r.sku());
            var body = price != null ? price + " cents" : "error unknown sku " + r.sku();
            replyChannel.add(new Reply(r.id(), body));
        }

        for (var reply : replyChannel) {
            var sku = pending.remove(reply.correlationId());
            if (sku == null) {
                System.out.println("no outstanding request for " + reply.correlationId() + ", dropped");
                continue;
            }
            System.out.println("reply for " + reply.correlationId() + " (" + sku + "): " + reply.body());
        }
        System.out.println("still waiting: " + pending.size());
    }
}
```

```output
no outstanding request for q0, dropped
reply for q3 (Z9): error unknown sku Z9
reply for q2 (B2): 500 cents
reply for q1 (A1): 1999 cents
still waiting: 0
```

- Verdict: idiom. A `HashMap` from the request id to the item is the table. The messages are records in memory, because Java has no JSON in the standard library.
- `pending.remove(id)` returns `null` when no request has that id, which is the case of a stray reply.
- `List.reversed`, from Java 21, gives the replier its different order of work.

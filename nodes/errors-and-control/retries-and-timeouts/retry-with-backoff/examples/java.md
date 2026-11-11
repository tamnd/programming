```java run id=idempotency
import java.util.HashMap;
import java.util.Map;
import java.util.Random;

public class Main {
    static class ConnectionLost extends RuntimeException {
        ConnectionLost(String message) { super(message); }
    }

    static class Server {
        int balance = 500;
        int calls = 0;
        final Map<String, String> receipts = new HashMap<>();

        String charge(String idempotencyKey, int amount) {
            calls++;
            String earlier = receipts.get(idempotencyKey);
            if (earlier != null) return earlier;
            balance -= amount;
            String receipt = "receipt " + (receipts.size() + 1);
            receipts.put(idempotencyKey, receipt);
            if (calls == 1) throw new ConnectionLost("connection reset before the response");
            return receipt;
        }
    }

    static long now = 0;

    static String chargeWithRetry(Server server, String key, int amount, Random rng) {
        long base = 100, cap = 1000;
        for (int attempt = 0; ; attempt++) {
            try {
                return server.charge(key, amount);
            } catch (ConnectionLost e) {
                if (attempt == 4) throw e;
                long delay = rng.nextLong(Math.min(cap, base << attempt) + 1);
                System.out.printf("t=%d ms: attempt %d failed (%s); wait %d ms%n", now, attempt + 1, e.getMessage(), delay);
                now += delay;
            }
        }
    }

    public static void main(String[] args) {
        Server server = new Server();
        Random rng = new Random(42);
        String key = "order-7781";
        System.out.println("result: " + chargeWithRetry(server, key, 100, rng));
        System.out.println("balance: " + server.balance + ", calls: " + server.calls);
        System.out.println("again: " + chargeWithRetry(server, key, 100, rng));
        System.out.println("balance: " + server.balance + ", calls: " + server.calls);
    }
}
```

```output
t=0 ms: attempt 1 failed (connection reset before the response); wait 2 ms
result: receipt 1
balance: 400, calls: 2
again: receipt 1
balance: 400, calls: 3
```

- Verdict: stdlib. Java has no retry method in the standard library. The loop uses `java.util.Random` with a fixed seed for the jitter, and a counter in place of a clock.
- The client sends the same idempotency key with each attempt. The server stores the result of the first charge under the key, and it returns that result for a repeated request. The charge happens once, though the server receives it three times.
- A charge is not idempotent by nature. The key makes the operation safe to retry.

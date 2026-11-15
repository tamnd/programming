```java run id=add-a-program
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.function.Consumer;

public class Main {
    // The canonical message of the bus.
    record Message(String type, int order, int amountCents) {}

    static final Map<String, List<Consumer<Message>>> subscribers = new HashMap<>();

    static void subscribe(String type, Consumer<Message> handler) {
        subscribers.computeIfAbsent(type, k -> new ArrayList<>()).add(handler);
    }

    static void publish(Message m) {
        System.out.println("bus: " + m.type() + " order=" + m.order() + " amount_cents=" + m.amountCents());
        for (Consumer<Message> h : subscribers.getOrDefault(m.type(), List.of())) {
            h.accept(m);
        }
    }

    // The adapter of the shop reads "order 42 total 19.99".
    static void shopAdapter(String line) {
        System.out.println("shop sends: " + line);
        String[] f = line.split(" ");
        String[] total = f[3].split("\\.");
        int cents = Integer.parseInt(total[0]) * 100 + Integer.parseInt(total[1]);
        publish(new Message("OrderPlaced", Integer.parseInt(f[1]), cents));
    }

    public static void main(String[] args) {
        subscribe("OrderPlaced", m -> System.out.println(
                "billing: charge " + m.amountCents() + " cents for order " + m.order()));
        subscribe("OrderPlaced", m -> System.out.println("warehouse: SHIP|" + m.order()));
        shopAdapter("order 42 total 19.99");

        System.out.println("analytics joins the bus");
        int[] seen = {0};
        subscribe("OrderPlaced", m -> {
            seen[0]++;
            System.out.println("analytics: order " + m.order() + " counted, total orders " + seen[0]);
        });
        shopAdapter("order 43 total 5.00");
    }
}
```

```output
shop sends: order 42 total 19.99
bus: OrderPlaced order=42 amount_cents=1999
billing: charge 1999 cents for order 42
warehouse: SHIP|42
analytics joins the bus
shop sends: order 43 total 5.00
bus: OrderPlaced order=43 amount_cents=500
billing: charge 500 cents for order 43
warehouse: SHIP|43
analytics: order 43 counted, total orders 1
```

- Verdict: idiom. Java has no message bus in the standard library. A `Map` from a message type to a list of `Consumer` objects is the bus, and the record `Message` is the common data model.
- `seen` is an array of one `int`, because a lambda can only use local variables that do not change; the array does not change, only its element.
- With a broker, each program runs in its own process, and a Jakarta Messaging topic for each message type gives the same structure.

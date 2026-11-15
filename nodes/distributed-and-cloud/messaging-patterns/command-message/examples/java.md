```java run id=dispatch-commands
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.ObjectInputFilter;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.Serializable;
import java.util.ArrayDeque;
import java.util.HashMap;
import java.util.Map;
import java.util.Queue;

public class Main {
    sealed interface Command extends Serializable permits ReserveStock, ChargeCard {}

    record ReserveStock(String sku, int qty) implements Command {}

    record ChargeCard(int order, int cents) implements Command {}

    static final Map<String, Integer> stock = new HashMap<>(Map.of("A1", 5));

    static byte[] encode(Command c) throws IOException {
        ByteArrayOutputStream bytes = new ByteArrayOutputStream();
        try (ObjectOutputStream out = new ObjectOutputStream(bytes)) {
            out.writeObject(c);
        }
        return bytes.toByteArray();
    }

    static Command decode(byte[] raw) throws IOException, ClassNotFoundException {
        try (ObjectInputStream in = new ObjectInputStream(new ByteArrayInputStream(raw))) {
            // Accept only the command classes; reject every other class in the stream.
            in.setObjectInputFilter(ObjectInputFilter.Config.createFilter("Main$*;java.lang.*;!*"));
            return (Command) in.readObject();
        }
    }

    static String handle(Command c) {
        if (c instanceof ReserveStock r) {
            int left = stock.get(r.sku());
            if (left < r.qty()) {
                return "reserve " + r.qty() + " of " + r.sku() + ": not enough stock, " + left + " left";
            }
            stock.put(r.sku(), left - r.qty());
            return "reserve " + r.qty() + " of " + r.sku() + ": " + (left - r.qty()) + " left";
        }
        ChargeCard ch = (ChargeCard) c;
        return "charge " + ch.cents() + " cents for order " + ch.order() + ": done";
    }

    public static void main(String[] args) throws Exception {
        Queue<byte[]> channel = new ArrayDeque<>(); // the channel carries bytes
        channel.add(encode(new ReserveStock("A1", 2)));
        channel.add(encode(new ChargeCard(42, 1999)));
        channel.add(encode(new ReserveStock("A1", 9)));
        while (!channel.isEmpty()) {
            System.out.println(handle(decode(channel.poll())));
        }
    }
}
```

```output
reserve 2 of A1: 3 left
charge 1999 cents for order 42: done
reserve 9 of A1: not enough stock, 3 left
```

- Verdict: stdlib. Java serialization turns a record into bytes and back; this is the body of a Jakarta Messaging `ObjectMessage`.
- The sealed interface `Command` lists the commands, and `instanceof` with a pattern picks the handler.
- `setObjectInputFilter` with the pattern `Main$*;java.lang.*;!*` accepts only the classes of the program and of `java.lang`, and rejects all other classes. Without a filter, the bytes of a message can make the receiver create objects of any serializable class on its class path.

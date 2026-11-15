```java run id=survive-crash
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.Map;

public class Main {
    // A queue whose state is a log on disk: one line for each send and each ack.
    static final class DurableQueue {
        final Path path;
        final Map<String, String> pending = new LinkedHashMap<>();

        DurableQueue(Path path) throws IOException {
            this.path = path;
            if (Files.exists(path)) {
                for (String line : Files.readAllLines(path, StandardCharsets.UTF_8)) {
                    String[] p = line.split(" ", 3);
                    if (p[0].equals("send")) {
                        pending.put(p[1], p[2]);
                    } else {
                        pending.remove(p[1]);
                    }
                }
            }
        }

        void log(String line) throws IOException {
            try (FileOutputStream out = new FileOutputStream(path.toFile(), true)) {
                out.write((line + "\n").getBytes(StandardCharsets.UTF_8));
                out.getFD().sync();
            }
        }

        void send(String id, String body) throws IOException {
            log("send " + id + " " + body);
            pending.put(id, body);
        }

        void ack(String id) throws IOException {
            log("ack " + id);
            pending.remove(id);
        }
    }

    public static void main(String[] args) throws IOException {
        Path dir = Files.createTempDirectory("gd");
        Path path = dir.resolve("orders.log");

        Map<String, String> memory = new HashMap<>();
        DurableQueue durable = new DurableQueue(path);
        for (int i = 1; i <= 3; i++) {
            memory.put("m" + i, "order-" + i);
            durable.send("m" + i, "order-" + i);
        }
        System.out.println("before crash: memory " + memory.size() + ", durable " + durable.pending.size());

        memory = new HashMap<>(); // a crash loses what was only in memory
        durable = new DurableQueue(path);
        System.out.println("after crash: memory " + memory.size() + ", durable " + durable.pending.size());

        String id = durable.pending.keySet().iterator().next();
        System.out.println("receive " + id + " " + durable.pending.get(id) + ", ack");
        durable.ack(id);

        durable = new DurableQueue(path);
        System.out.println("after second crash: durable " + durable.pending.size() + " "
                + String.join(" ", durable.pending.keySet()));
        Files.delete(path);
        Files.delete(dir);
    }
}
```

```output
before crash: memory 3, durable 3
after crash: memory 0, durable 3
receive m1 order-1, ack
after second crash: durable 2 m2 m3
```

- Verdict: idiom. The Java standard library has no persistent queue. A log file with `FileDescriptor.sync` after each line is the store.
- A `LinkedHashMap` keeps the pending messages in the order of the log, so the receiver takes the oldest one first.
- In Jakarta Messaging, the same choice is the delivery mode of the producer, `DeliveryMode.PERSISTENT`, and the provider does the writes.

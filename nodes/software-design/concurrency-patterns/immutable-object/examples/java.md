```java run id=immutable-object
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.atomic.AtomicInteger;

public class Main {
    static final class Settings { // final class, final fields, no setters
        private final int port;
        private final String tag;

        Settings(int port, String tag) {
            this.port = port;
            this.tag = tag;
        }

        int port() { return port; }
        String tag() { return tag; }
    }

    static final Settings first = new Settings(8000, "port 8000");
    static volatile Settings current = first; // the writer replaces the reference

    public static void main(String[] args) throws InterruptedException {
        AtomicInteger reads = new AtomicInteger();
        AtomicInteger mixed = new AtomicInteger();
        List<Thread> threads = new ArrayList<>();
        for (int i = 0; i < 4; i++) {
            threads.add(new Thread(() -> {
                for (int r = 0; r < 1000; r++) {
                    Settings s = current; // one read gives a consistent snapshot
                    Thread.yield();
                    if (!s.tag().equals("port " + s.port())) mixed.incrementAndGet();
                    reads.incrementAndGet();
                }
            }));
        }
        threads.add(new Thread(() -> {
            for (int port = 8001; port <= 8100; port++) {
                current = new Settings(port, "port " + port); // a new object, never a change
                Thread.yield();
            }
        }));
        for (Thread t : threads) t.start();
        for (Thread t : threads) t.join();
        boolean unchanged = first.port() == 8000 && first.tag().equals("port 8000");
        System.out.println("reads: " + reads.get());
        System.out.println("reads that saw a mixed pair: " + mixed.get());
        System.out.println("settings published: " + (1 + current.port() - first.port()));
        System.out.println("first settings unchanged: " + (unchanged ? "True" : "False"));
    }
}
```

```output
reads: 4000
reads that saw a mixed pair: 0
settings published: 101
first settings unchanged: True
```

- Verdict: native. `Settings` has only `final` fields, which the Java memory model makes safe to read from any thread after the constructor ends, and `volatile` publishes each new reference.

```java run id=pool
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.List;
import java.util.function.Consumer;
import java.util.function.IntFunction;

public class Main {
    static final class Buffer {
        final int id; // a real buffer would be costly to create
        final StringBuilder text = new StringBuilder();

        Buffer(int id) {
            this.id = id;
        }
    }

    static final class Pool<T> {
        private final ArrayDeque<T> idle = new ArrayDeque<>();
        private final IntFunction<T> create;
        private final Consumer<T> reset;
        private final int maxIdle;
        private int created;

        Pool(IntFunction<T> create, Consumer<T> reset, int maxIdle) {
            this.create = create;
            this.reset = reset;
            this.maxIdle = maxIdle;
        }

        synchronized T acquire() {
            T item = idle.pollFirst();
            if (item != null) {
                return item;
            }
            created++;
            return create.apply(created);
        }

        void release(T item) {
            reset.accept(item); // clear the state before the next user sees it
            synchronized (this) {
                if (idle.size() < maxIdle) {
                    idle.addFirst(item);
                }
            }
        }

        synchronized int idle() {
            return idle.size();
        }

        synchronized int created() {
            return created;
        }
    }

    public static void main(String[] args) {
        Pool<Buffer> pool = new Pool<>(Buffer::new, b -> b.text.setLength(0), 2);
        for (int i = 1; i <= 3; i++) {
            Buffer b = pool.acquire();
            try {
                b.text.append("request ").append(i);
                System.out.println("request " + i + " -> buffer #" + b.id + ": " + b.text);
            } finally {
                pool.release(b); // runs even if the body throws
            }
        }

        List<Buffer> held = new ArrayList<>();
        for (int i = 0; i < 3; i++) {
            held.add(pool.acquire());
        }
        StringBuilder ids = new StringBuilder();
        held.stream().mapToInt(b -> b.id).sorted().forEach(id -> ids.append(" #").append(id));
        System.out.println("3 at once -> buffers" + ids);
        held.forEach(pool::release);
        System.out.println("idle after release: " + pool.idle());
        System.out.println("created: " + pool.created());
    }
}
```

```output
request 1 -> buffer #1: request 1
request 2 -> buffer #1: request 2
request 3 -> buffer #1: request 3
3 at once -> buffers #1 #2 #3
idle after release: 2
created: 3
```

- Verdict: library. The JDK has thread pools (`java.util.concurrent.Executors`) but no general object pool. Apache Commons Pool gives one, and JDBC connection pools are the common case.
- The `synchronized` methods make the pool safe for many threads. The reset runs outside the lock, because it touches only the item.
- A modern JVM allocates small objects fast, so pool only objects with a real creation cost: connections, threads, large buffers.

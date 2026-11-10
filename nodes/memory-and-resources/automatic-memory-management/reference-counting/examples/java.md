```java run id=shared-connection
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.atomic.AtomicInteger;

public class Main {
    static class Connection {
        private final AtomicInteger users = new AtomicInteger(1);
        private volatile boolean open = true;

        Connection retain() {
            users.incrementAndGet();
            return this;
        }

        void release() {
            if (users.decrementAndGet() == 0) {
                open = false;
                System.out.println("connection closed");
            }
        }
    }

    public static void main(String[] args) throws InterruptedException {
        Connection conn = new Connection();
        List<Thread> workers = new ArrayList<>();
        for (int i = 0; i < 4; i++) {
            Connection mine = conn.retain();
            workers.add(Thread.ofPlatform().start(mine::release));
        }
        for (Thread worker : workers) {
            worker.join();
        }
        System.out.println("open after workers: " + conn.open);
        conn.release();
        System.out.println("open after last release: " + conn.open);
    }
}
```

```output
open after workers: true
connection closed
open after last release: false
```

- Verdict: idiom. The JVM frees memory with a tracing collector, so Java has no reference counts for objects. A count is still useful for a resource that must close when its last user is done.
- `AtomicInteger.decrementAndGet` changes the count as one atomic step, so two threads cannot both see the same old value.
- Each worker gets its reference before its thread starts, so the count cannot reach zero while a worker still waits to run.
- For a resource with one owner, try-with-resources is the usual form; see [[resource-acquisition-is-initialization]].

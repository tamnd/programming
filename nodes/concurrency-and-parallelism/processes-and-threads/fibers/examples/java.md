```java run id=virtual-thread-fringe
import java.util.List;
import java.util.concurrent.SynchronousQueue;

public class Main {
    static final Object END = new Object();

    // Start a walk on a virtual thread. The thread hands each leaf over the queue
    // and waits there until the caller takes it.
    static SynchronousQueue<Object> start(Object tree) {
        SynchronousQueue<Object> queue = new SynchronousQueue<>();
        Thread.ofVirtual().start(() -> {
            try {
                walk(tree, queue);
                queue.put(END);
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        });
        return queue;
    }

    static void walk(Object tree, SynchronousQueue<Object> queue) throws InterruptedException {
        if (tree instanceof List<?> list) {
            for (Object child : list) {
                walk(child, queue);
            }
        } else {
            queue.put(tree); // waits here, deep in the recursion
        }
    }

    static boolean sameFringe(Object x, Object y) throws InterruptedException {
        SynchronousQueue<Object> qx = start(x);
        SynchronousQueue<Object> qy = start(y);
        while (true) {
            Object a = qx.take();
            Object b = qy.take();
            if (!a.equals(b)) return false;
            if (a == END) return true;
        }
    }

    public static void main(String[] args) throws InterruptedException {
        Object a = List.of(List.of(1, 2), List.of(3, List.of(4)));
        Object b = List.of(1, List.of(2, List.of(3, 4)));
        Object c = List.of(List.of(1, 2), List.of(3, 5));

        SynchronousQueue<Object> leaves = start(a);
        for (Object leaf = leaves.take(); leaf != END; leaf = leaves.take()) {
            System.out.println("got " + leaf);
        }
        System.out.println("same fringe a b: " + sameFringe(a, b));
        System.out.println("same fringe a c: " + sameFringe(a, c));
    }
}
```

```output for=virtual-thread-fringe
got 1
got 2
got 3
got 4
same fringe a b: true
same fringe a c: false
```

- Verdict: absent. Java has no fiber type. A virtual thread with a `SynchronousQueue` gives the same stackful pause: `walk` can wait in `put` at any depth of the recursion.
- A `SynchronousQueue` has no capacity, so each `put` waits until the caller takes the value. The two sides take turns, as a fiber and its caller do.
- The difference from a fiber is the scheduler. The caller does not switch to the walk; the JDK decides when the virtual thread runs.
- When `sameFringe` returns early, the unfinished walks stay blocked in `put`. Virtual threads are daemon threads, so they do not stop the JVM from exiting.

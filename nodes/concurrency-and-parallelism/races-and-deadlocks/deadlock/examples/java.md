```java run id=timed
import java.util.concurrent.CyclicBarrier;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.ReentrantLock;

public class Main {
    static final CyclicBarrier bothHold = new CyclicBarrier(2);
    static final CyclicBarrier bothTried = new CyclicBarrier(2);
    static final boolean[] got = new boolean[2];

    static Thread worker(int index, ReentrantLock first, ReentrantLock second) {
        return new Thread(() -> {
            first.lock();
            try {
                bothHold.await();
                got[index] = second.tryLock(200, TimeUnit.MILLISECONDS);
                if (got[index]) second.unlock();
                bothTried.await();
            } catch (Exception e) {
                throw new RuntimeException(e);
            } finally {
                first.unlock();
            }
        });
    }

    public static void main(String[] args) throws InterruptedException {
        ReentrantLock a = new ReentrantLock();
        ReentrantLock b = new ReentrantLock();
        Thread t1 = worker(0, a, b);
        Thread t2 = worker(1, b, a);
        t1.start();
        t2.start();
        t1.join();
        t2.join();
        System.out.println("t1 got its second lock: " + got[0]);
        System.out.println("t2 got its second lock: " + got[1]);
    }
}
```

```output for=timed
t1 got its second lock: false
t2 got its second lock: false
```

- Verdict: native. `synchronized` blocks lock the monitor of an object, and the language neither prevents nor detects deadlock. `java.util.concurrent.locks` adds `ReentrantLock` with `tryLock` and a timeout.
- A `synchronized` block cannot time out. Code that needs the back off method uses `ReentrantLock`, as here.
- `ThreadMXBean.findDeadlockedThreads()` finds a cycle of platform threads at run time (see the Java program in the node). The `jstack -l` tool prints the threads and their locks.
- Monitors and `ReentrantLock` are reentrant, so a thread that takes the same lock twice does not deadlock.

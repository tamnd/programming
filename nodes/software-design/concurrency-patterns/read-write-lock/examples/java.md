```java run id=read-write-lock
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.CyclicBarrier;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.locks.ReentrantReadWriteLock;

public class Main {
    static final ReentrantReadWriteLock lock = new ReentrantReadWriteLock();
    static final int[] pair = {0, 0}; // the writer keeps the two values equal
    static final AtomicInteger inside = new AtomicInteger();
    static final AtomicInteger reads = new AtomicInteger();
    static final AtomicInteger mixed = new AtomicInteger();
    static int writes = 0;
    static int overlap = 0;

    static void runAll(List<Thread> threads) throws InterruptedException {
        for (Thread t : threads) t.start();
        for (Thread t : threads) t.join();
    }

    public static void main(String[] args) throws Exception {
        // Part 1: two readers hold the read lock at the same time.
        CyclicBarrier meeting = new CyclicBarrier(2);
        AtomicInteger met = new AtomicInteger();
        List<Thread> meets = new ArrayList<>();
        for (int i = 0; i < 2; i++) {
            meets.add(new Thread(() -> {
                lock.readLock().lock();
                try {
                    meeting.await(5, TimeUnit.SECONDS); // returns only when both readers are inside
                    met.incrementAndGet();
                } catch (Exception e) {
                    // the other reader did not arrive in time
                } finally {
                    lock.readLock().unlock();
                }
            }));
        }
        runAll(meets);

        // Part 2: readers and a writer share the pair.
        List<Thread> threads = new ArrayList<>();
        for (int r = 0; r < 4; r++) {
            threads.add(new Thread(() -> {
                for (int n = 0; n < 1000; n++) {
                    lock.readLock().lock();
                    try {
                        inside.incrementAndGet();
                        int x = pair[0];
                        Thread.yield(); // give the writer a chance to run here
                        int y = pair[1];
                        inside.decrementAndGet();
                        reads.incrementAndGet();
                        if (x != y) mixed.incrementAndGet();
                    } finally {
                        lock.readLock().unlock();
                    }
                }
            }));
        }
        threads.add(new Thread(() -> {
            for (int i = 1; i <= 100; i++) {
                lock.writeLock().lock();
                try {
                    if (inside.get() > 0) overlap++;
                    pair[0] = i;
                    Thread.yield();
                    pair[1] = i;
                    writes++;
                } finally {
                    lock.writeLock().unlock();
                }
            }
        }));
        runAll(threads);

        System.out.println("two readers inside at once: " + (met.get() == 2 ? "True" : "False"));
        System.out.println("reads: " + reads.get());
        System.out.println("reads that saw a mixed pair: " + mixed.get());
        System.out.println("writes: " + writes);
        System.out.println("writes that ran with a reader inside: " + overlap);
    }
}
```

```output
two readers inside at once: True
reads: 4000
reads that saw a mixed pair: 0
writes: 100
writes that ran with a reader inside: 0
```

- Verdict: stdlib. `ReentrantReadWriteLock` in `java.util.concurrent.locks` gives a read lock and a write lock.

```java run id=three-ways
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.ReentrantLock;
import java.util.function.IntConsumer;

public class Main {
    static final int THREADS = 4;
    static final int STEPS = 1000;

    static void runThreads(IntConsumer body) throws InterruptedException {
        List<Thread> threads = new ArrayList<>();
        for (int i = 0; i < THREADS; i++) {
            int id = i;
            threads.add(new Thread(() -> body.accept(id)));
        }
        for (Thread t : threads) t.start();
        for (Thread t : threads) t.join();
    }

    static int count = 0;

    public static void main(String[] args) throws Exception {
        // 1. Synchronization: a lock guards the shared counter.
        ReentrantLock lock = new ReentrantLock();
        runThreads(i -> {
            for (int n = 0; n < STEPS; n++) {
                lock.lock();
                try {
                    count++;
                } finally {
                    lock.unlock();
                }
            }
        });
        System.out.println("lock around a shared counter: " + count);

        // 2. Confinement: one owner thread keeps the counter; the others send it messages.
        BlockingQueue<Integer> inbox = new LinkedBlockingQueue<>();
        int[] owned = new int[1];
        Thread owner = new Thread(() -> {
            int total = 0;
            try {
                for (int n = 0; n < THREADS * STEPS; n++) total += inbox.poll(5, TimeUnit.SECONDS);
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
            owned[0] = total;
        });
        owner.start();
        runThreads(i -> {
            for (int n = 0; n < STEPS; n++) inbox.add(1);
        });
        owner.join(); // join makes the owner's write visible here
        System.out.println("one owner thread, the others send messages: " + owned[0]);

        // 3. No sharing: each thread makes its own result, and the results are combined after the join.
        int[] parts = new int[THREADS];
        runThreads(i -> {
            for (int n = 0; n < STEPS; n++) parts[i]++; // only thread i writes slot i
        });
        int sum = 0;
        for (int p : parts) sum += p;
        System.out.println("own result for each thread, combined after the join: " + sum);
    }
}
```

```output
lock around a shared counter: 4000
one owner thread, the others send messages: 4000
own result for each thread, combined after the join: 4000
```

- Verdict: native. A `ReentrantLock` from `java.util.concurrent.locks`, a `LinkedBlockingQueue` for the messages, and an array slot for each thread. `join` makes the writes of a thread visible to the thread that joins it.

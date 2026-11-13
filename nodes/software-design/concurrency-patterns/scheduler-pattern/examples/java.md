```java run id=scheduler-pattern
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;
import java.util.concurrent.atomic.AtomicInteger;

public class Main {
    record Entry(int priority, int arrival, String name) {}

    // Decides which waiting thread uses the printer next.
    static class Scheduler {
        private boolean busy = false;
        private final List<Entry> waiting = new ArrayList<>();
        private int arrivals = 0;
        private Entry chosen = null;

        synchronized void enter(String name, int priority) throws InterruptedException {
            if (!busy && waiting.isEmpty()) {
                busy = true;
                return;
            }
            Entry entry = new Entry(priority, ++arrivals, name);
            waiting.add(entry);
            notifyAll();
            // Sleep until the scheduler chooses this thread.
            while (chosen != entry) wait();
            chosen = null;
        }

        synchronized void done() {
            if (waiting.isEmpty()) {
                busy = false;
                return;
            }
            // The policy: the lowest priority number first, then the first to arrive.
            Entry entry = waiting.stream()
                .min(Comparator.comparingInt(Entry::priority).thenComparingInt(Entry::arrival))
                .get();
            waiting.remove(entry);
            chosen = entry; // the printer stays busy and passes to that thread
            notifyAll();
        }

        synchronized int waitUntilWaiting(int n) throws InterruptedException {
            long end = System.currentTimeMillis() + 5000;
            while (waiting.size() < n && System.currentTimeMillis() < end) wait(100);
            return waiting.size();
        }
    }

    static final Scheduler scheduler = new Scheduler();
    static final List<String> order = Collections.synchronizedList(new ArrayList<>());
    static final AtomicInteger onPrinter = new AtomicInteger();
    static final AtomicInteger mostOnPrinter = new AtomicInteger();

    static void usePrinter(String name) {
        mostOnPrinter.accumulateAndGet(onPrinter.incrementAndGet(), Math::max);
        order.add(name);
        onPrinter.decrementAndGet();
    }

    public static void main(String[] args) throws Exception {
        scheduler.enter("first", 0); // the first job takes the free printer
        usePrinter("first");
        String[] names = {"report", "invoice", "photo", "label"};
        int[] priorities = {2, 1, 3, 1};
        List<Thread> threads = new ArrayList<>();
        for (int i = 0; i < names.length; i++) {
            String name = names[i];
            int priority = priorities[i];
            Thread t = new Thread(() -> {
                try {
                    scheduler.enter(name, priority);
                    usePrinter(name);
                    scheduler.done();
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
            });
            t.start();
            threads.add(t);
            scheduler.waitUntilWaiting(i + 1); // start the jobs in a known order
        }
        System.out.println("jobs waiting when the first job ends: " + scheduler.waitUntilWaiting(names.length));
        scheduler.done();
        for (Thread t : threads) t.join();

        System.out.println("print order: " + String.join(", ", order));
        System.out.println("jobs on the printer at once, at most: " + mostOnPrinter.get());
    }
}
```

```output
jobs waiting when the first job ends: 4
print order: first, invoice, label, report, photo
jobs on the printer at once, at most: 1
```

- Verdict: idiom. A class with `synchronized` methods, `wait()` in a `while` loop and `notifyAll()`. A `ReentrantLock` or `Semaphore` in fair mode gives only the FIFO policy.

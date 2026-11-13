```java run id=worker-queue
import java.util.ArrayDeque;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

public class Main {
    static final int TASKS = 10;
    static final int WORKERS = 3;
    static final int FAILS_ON = 4; // the first worker that takes this task fails before it does the work

    // Hands out tasks and keeps each one until a worker acknowledges it.
    static class WorkQueue {
        private final ArrayDeque<Integer> ready = new ArrayDeque<>();
        private final Map<Integer, Integer> unacked = new HashMap<>(); // delivery tag -> task
        int pending = 0; // tasks put and not yet acknowledged
        private int tag = 0;
        int deliveries = 0;
        int putBack = 0;

        synchronized void put(int task) {
            ready.addLast(task);
            pending++;
            notifyAll();
        }

        // Returns {tag, task}, or null when every task is acknowledged.
        synchronized int[] get() throws InterruptedException {
            while (ready.isEmpty() && pending > 0) wait(5000);
            if (ready.isEmpty()) return null;
            tag++;
            deliveries++;
            unacked.put(tag, ready.removeFirst());
            return new int[] {tag, unacked.get(tag)};
        }

        synchronized void ack(int tag) {
            unacked.remove(tag);
            pending--;
            notifyAll();
        }

        // The worker went away without an ack, so the task goes back on the queue.
        synchronized void workerLost(int tag) {
            ready.addLast(unacked.remove(tag));
            putBack++;
            notifyAll();
        }
    }

    static final WorkQueue work = new WorkQueue();
    static final TreeMap<Integer, Integer> results = new TreeMap<>();
    static boolean failed = false;

    static void worker() {
        try {
            int[] delivery;
            while ((delivery = work.get()) != null) {
                int tag = delivery[0], task = delivery[1];
                boolean fail;
                synchronized (results) {
                    fail = task == FAILS_ON && !failed;
                    if (fail) failed = true;
                }
                if (fail) {
                    work.workerLost(tag); // this worker stops here
                    return;
                }
                synchronized (results) {
                    results.put(task, task * task); // the work
                }
                work.ack(tag); // only after the work is done
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }

    public static void main(String[] args) throws InterruptedException {
        for (int task = 1; task <= TASKS; task++) work.put(task);
        List<Thread> threads = new ArrayList<>();
        for (int i = 0; i < WORKERS; i++) threads.add(new Thread(Main::worker));
        for (Thread t : threads) t.start();
        for (Thread t : threads) t.join();

        boolean all = results.size() == TASKS && results.firstKey() == 1 && results.lastKey() == TASKS;
        System.out.println("tasks put on the queue: " + TASKS);
        System.out.println("workers: " + WORKERS);
        System.out.println("deliveries: " + work.deliveries);
        System.out.println("tasks put back after a worker failed: " + work.putBack);
        System.out.println("tasks still not acknowledged: " + work.pending);
        System.out.println("every task has a result: " + (all ? "True" : "False"));
    }
}
```

```output
tasks put on the queue: 10
workers: 3
deliveries: 11
tasks put back after a worker failed: 1
tasks still not acknowledged: 0
every task has a result: True
```

- Verdict: idiom. A `WorkQueue` class with `synchronized` methods and `wait` and `notifyAll`. A `BlockingQueue` alone forgets a task when a worker takes it.

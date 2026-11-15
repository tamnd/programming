```java run id=competing-workers
import java.util.*;
import java.util.concurrent.*;

public class Main {
    static final int MAX_DELIVERIES = 3, WORKERS = 3;
    record Delivery(int job, int attempt) {}
    record Event(int job, int attempt, String text) {}

    static final BlockingQueue<Delivery> jobs = new LinkedBlockingQueue<>();
    static final Delivery STOP = new Delivery(0, 0);
    static final Map<Integer, Integer> results = new TreeMap<>();
    static final List<Event> events = new ArrayList<>();
    static int pending = 8, deliveries = 0;

    static int handle(int job, int attempt) throws Exception {
        Thread.sleep(40);
        if (job == 4 && attempt == 1) throw new Exception("timeout of the database");
        if (job == 7) throw new Exception("bad payload");
        return job * job;
    }

    static void worker() {
        try {
            while (true) {
                Delivery d = jobs.take();
                if (d == STOP) return;
                synchronized (Main.class) { deliveries++; }
                Integer result = null;
                Exception error = null;
                try { result = handle(d.job(), d.attempt()); } catch (Exception e) { error = e; }
                synchronized (Main.class) {
                    if (error != null && d.attempt() < MAX_DELIVERIES) {
                        events.add(new Event(d.job(), d.attempt(), "failed on delivery " + d.attempt()
                                + " (" + error.getMessage() + "), back on the queue"));
                        jobs.add(new Delivery(d.job(), d.attempt() + 1));
                        continue;
                    }
                    if (error != null) {
                        events.add(new Event(d.job(), d.attempt(), "failed on delivery " + d.attempt()
                                + " (" + error.getMessage() + "), to the dead letter channel"));
                    } else {
                        results.put(d.job(), result);
                    }
                    if (--pending == 0) {
                        for (int i = 0; i < WORKERS; i++) jobs.add(STOP);
                    }
                }
            }
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }

    public static void main(String[] args) throws InterruptedException {
        for (int job = 1; job <= 8; job++) jobs.add(new Delivery(job, 1));
        List<Thread> threads = new ArrayList<>();
        for (int i = 0; i < WORKERS; i++) {
            Thread t = new Thread(Main::worker);
            t.start();
            threads.add(t);
        }
        for (Thread t : threads) t.join();

        System.out.println(WORKERS + " workers, 8 jobs on one queue");
        events.sort(Comparator.comparingInt(Event::job).thenComparingInt(Event::attempt));
        for (Event e : events) System.out.println("job " + e.job() + ": " + e.text());
        StringJoiner values = new StringJoiner(" ");
        results.values().forEach(v -> values.add(v.toString()));
        System.out.println("done: " + results.size() + " jobs, results " + values);
        System.out.println("deliveries: " + deliveries);
    }
}
```

```output
3 workers, 8 jobs on one queue
job 4: failed on delivery 1 (timeout of the database), back on the queue
job 7: failed on delivery 1 (bad payload), back on the queue
job 7: failed on delivery 2 (bad payload), back on the queue
job 7: failed on delivery 3 (bad payload), to the dead letter channel
done: 7 jobs, results 1 4 9 16 25 36 64
deliveries: 11
```

- Verdict: idiom. A `LinkedBlockingQueue` with three threads that call `take` is the standard form; in a broker client, `concurrentConsumers` or a listener container does the same.
- The `STOP` record is a poison pill: the last finished job puts one for each thread.
- The shared counters and lists are changed only inside `synchronized`, so the threads do not lose updates.

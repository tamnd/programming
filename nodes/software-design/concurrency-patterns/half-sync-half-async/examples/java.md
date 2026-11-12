```java run id=hsha
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.atomic.AtomicLong;

public class Main {
    static final LinkedBlockingQueue<Integer> requests = new LinkedBlockingQueue<>(); // queueing layer
    static final CountDownLatch gate = new CountDownLatch(1); // lets the demo hold the sync layer
    static final AtomicInteger done = new AtomicInteger();
    static final AtomicLong sum = new AtomicLong();
    static int received = 0;

    // Async layer: a short handler that never blocks.
    static void onEvent(int eventId) {
        received++;
        requests.offer(eventId);
    }

    public static void main(String[] args) throws InterruptedException {
        List<Thread> workers = new ArrayList<>();
        for (int i = 0; i < 4; i++) {
            Thread w = new Thread(() -> {
                try {
                    gate.await();
                    while (true) {
                        int request = requests.take(); // the sync layer blocks here
                        if (request < 0) return;
                        Thread.sleep(1); // a blocking call, such as a database query
                        done.incrementAndGet();
                        sum.addAndGet(request);
                    }
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
            });
            w.start();
            workers.add(w);
        }
        Thread eventSource = new Thread(() -> {
            for (int id = 0; id < 100; id++) onEvent(id);
        });
        eventSource.start();
        eventSource.join();
        System.out.println("events received by the async layer: " + received);
        System.out.println("requests in the queueing layer before the sync layer starts: " + requests.size());
        gate.countDown();
        for (int i = 0; i < 4; i++) requests.put(-1);
        for (Thread w : workers) w.join();
        System.out.println("requests done by the sync layer: " + done.get());
        System.out.println("sum of request ids: " + sum.get());
    }
}
```

```output
events received by the async layer: 100
requests in the queueing layer before the sync layer starts: 100
requests done by the sync layer: 100
sum of request ids: 4950
```

- Verdict: stdlib. `LinkedBlockingQueue` is the queueing layer: `offer` returns at once for the event handler, and `take` blocks the worker threads.

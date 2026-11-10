```java run id=queues
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.SynchronousQueue;
import java.util.concurrent.TimeUnit;

public class Main {
    static final int DONE = -1; // the poison pill: a value that means "no more"

    public static void main(String[] args) throws InterruptedException {
        BlockingQueue<Integer> jobs = new ArrayBlockingQueue<>(2);
        Thread[] producers = new Thread[2];
        for (int p = 0; p < 2; p++) {
            int id = p + 1;
            producers[p] = Thread.ofPlatform().start(() -> {
                try {
                    for (int i = 1; i <= 3; i++) {
                        jobs.put(id * 10 * i); // blocks while the queue is full
                    }
                    jobs.put(DONE);
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
            });
        }
        int total = 0;
        int finished = 0;
        while (finished < producers.length) {
            int v = jobs.take();
            if (v == DONE) finished++;
            else total += v;
        }
        for (Thread t : producers) t.join();
        System.out.println("total: " + total);

        SynchronousQueue<String> handoff = new SynchronousQueue<>();
        System.out.println("offer with no taker: " + handoff.offer("x"));
        Thread taker = Thread.ofPlatform().start(() -> {
            try {
                System.out.println("took: " + handoff.take());
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        });
        handoff.put("ping"); // waits for the taker
        taker.join();
        System.out.println("poll: " + jobs.poll(50, TimeUnit.MILLISECONDS));
    }
}
```

```output for=queues
total: 180
offer with no taker: false
took: ping
poll: null
```

- Verdict: stdlib. `java.util.concurrent.BlockingQueue` is the channel. `SynchronousQueue` has no capacity, so each `put` waits for a `take`, as in a rendezvous.
- A `BlockingQueue` cannot be closed. The producers send an agreed "poison" value to say that they have finished.
- The interface has no operation that waits on several queues at once. `poll` with a time limit gives a receive with a timeout.

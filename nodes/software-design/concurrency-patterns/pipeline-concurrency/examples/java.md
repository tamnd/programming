```java run id=pipeline-concurrency
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.atomic.AtomicBoolean;

public class Main {
    static final int ITEMS = 20;
    static final int WORKERS = 3;
    static final int DONE = -1; // tells the next stage that no more values come

    public static void main(String[] args) throws Exception {
        BlockingQueue<Integer> numbers = new ArrayBlockingQueue<>(1); // source -> square workers
        BlockingQueue<Integer> squares = new ArrayBlockingQueue<>(1); // square workers -> sink
        AtomicBoolean sinkStarted = new AtomicBoolean(false);
        boolean[] overlap = new boolean[1];

        List<Thread> stages = new ArrayList<>();
        stages.add(new Thread(() -> {
            try {
                for (int n = 1; n <= ITEMS; n++) numbers.put(n); // blocks while the buffer is full
                // Each stage can hold only a few values, so the sink must already be running.
                overlap[0] = sinkStarted.get();
                for (int w = 0; w < WORKERS; w++) numbers.put(DONE);
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        }));
        for (int w = 0; w < WORKERS; w++) { // fan-out
            stages.add(new Thread(() -> {
                try {
                    int n;
                    while ((n = numbers.take()) != DONE) squares.put(n * n);
                    squares.put(DONE); // this worker is done
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
            }));
        }
        for (Thread t : stages) t.start();

        // The sink reads the outputs of all workers from one queue: fan-in.
        int received = 0, total = 0, finished = 0;
        while (finished < WORKERS) {
            int value = squares.take();
            sinkStarted.set(true);
            if (value == DONE) {
                finished++;
            } else {
                received++;
                total += value;
            }
        }
        for (Thread t : stages) t.join();

        System.out.println("items from the source: " + ITEMS);
        System.out.println("square workers: " + WORKERS);
        System.out.println("squares that reached the sink: " + received);
        System.out.println("sum of squares: " + total);
        System.out.println("the sink got a value before the source sent its last: " + (overlap[0] ? "True" : "False"));
    }
}
```

```output
items from the source: 20
square workers: 3
squares that reached the sink: 20
sum of squares: 2870
the sink got a value before the source sent its last: True
```

- Verdict: stdlib. `ArrayBlockingQueue` with a capacity of 1 links the stages, and a `DONE` value per worker ends the stream.

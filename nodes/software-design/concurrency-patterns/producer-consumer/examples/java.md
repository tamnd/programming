```java run id=producer-consumer
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;

public class Main {
    static final Integer DONE = -1; // the end-of-stream object

    public static void main(String[] args) throws InterruptedException {
        BlockingQueue<Integer> buffer = new ArrayBlockingQueue<>(4);
        List<Integer> consumed = Collections.synchronizedList(new ArrayList<>());
        List<Thread> producers = new ArrayList<>();
        List<Thread> consumers = new ArrayList<>();
        for (int s : new int[] {0, 500}) {
            producers.add(new Thread(() -> {
                try {
                    for (int item = s; item < s + 500; item++) buffer.put(item);
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
            }));
        }
        for (int i = 0; i < 2; i++) {
            consumers.add(new Thread(() -> {
                try {
                    while (true) {
                        Integer item = buffer.take();
                        if (item.equals(DONE)) return;
                        consumed.add(item);
                    }
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
            }));
        }
        for (Thread t : producers) t.start();
        for (Thread t : consumers) t.start();
        for (Thread t : producers) t.join();
        for (Thread t : consumers) buffer.put(DONE); // one for each consumer
        for (Thread t : consumers) t.join();

        List<Integer> sorted = new ArrayList<>(consumed);
        Collections.sort(sorted);
        boolean once = sorted.size() == 1000;
        for (int i = 0; once && i < 1000; i++) once = sorted.get(i) == i;
        long sum = 0;
        for (int x : sorted) sum += x;
        System.out.println("items consumed: " + consumed.size());
        System.out.println("sum of items: " + sum);
        System.out.println("each item consumed once: " + (once ? "True" : "False"));
    }
}
```

```output
items consumed: 1000
sum of items: 499500
each item consumed once: True
```

- Verdict: stdlib. `ArrayBlockingQueue` with capacity 4 is the bounded buffer: `put` waits when it is full and `take` waits when it is empty.

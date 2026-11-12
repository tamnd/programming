```java run id=monitor
import java.util.ArrayDeque;

public class Main {
    // Every Java object is a monitor: synchronized methods take its lock,
    // and wait and notifyAll use its single wait queue.
    static class MessageQueue {
        private final int capacity;
        private final ArrayDeque<Integer> items = new ArrayDeque<>();
        int puts = 0;
        int gets = 0;
        int largest = 0;

        MessageQueue(int capacity) {
            this.capacity = capacity;
        }

        synchronized void put(int message) throws InterruptedException {
            while (items.size() == capacity) {
                wait(); // releases the monitor lock while it waits
            }
            items.addLast(message);
            puts++;
            largest = Math.max(largest, items.size());
            notifyAll(); // one wait queue holds both kinds of waiters
        }

        synchronized int get() throws InterruptedException {
            while (items.isEmpty()) {
                wait();
            }
            int message = items.removeFirst();
            gets++;
            notifyAll();
            return message;
        }
    }

    public static void main(String[] args) throws InterruptedException {
        MessageQueue q = new MessageQueue(2);
        long[] sums = new long[2];
        Thread[] threads = new Thread[6];
        for (int p = 0; p < 4; p++) {
            final int base = p * 250;
            threads[p] = new Thread(() -> {
                try {
                    for (int i = 0; i < 250; i++) q.put(base + i);
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
            });
        }
        for (int c = 0; c < 2; c++) {
            final int slot = c;
            threads[4 + c] = new Thread(() -> {
                try {
                    for (int i = 0; i < 500; i++) sums[slot] += q.get();
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
            });
        }
        for (Thread t : threads) t.start();
        for (Thread t : threads) t.join();
        synchronized (q) {
            System.out.println("messages put: " + q.puts);
            System.out.println("messages got: " + q.gets);
            System.out.println("sum of messages got: " + (sums[0] + sums[1]));
            System.out.println("queue never held more than 2: " + (q.largest <= 2 ? "True" : "False"));
        }
    }
}
```

```output
messages put: 1000
messages got: 1000
sum of messages got: 499500
queue never held more than 2: True
```

- Verdict: native. `synchronized`, `wait` and `notifyAll` are part of the language and of `Object`. The single wait queue holds producers and consumers, so the code calls `notifyAll` and each thread checks its condition again.

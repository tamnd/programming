```java run id=blocking-queue-inbox
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.SynchronousQueue;

public class Main {
    record Message(String kind, int value) {}

    public static void main(String[] args) throws InterruptedException {
        // A worker thread owns the total and changes it only when a message arrives.
        BlockingQueue<Message> inbox = new LinkedBlockingQueue<>();
        BlockingQueue<Integer> replies = new LinkedBlockingQueue<>();
        Thread worker = new Thread(() -> {
            int total = 0;
            try {
                while (true) {
                    Message m = inbox.take();
                    switch (m.kind()) {
                        case "add" -> total += m.value();
                        case "get" -> replies.put(total);
                        case "stop" -> {
                            return;
                        }
                        default -> throw new IllegalStateException(m.kind());
                    }
                }
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        });
        worker.start();

        // Four senders put messages on the queue. They share no counter, so they need no lock.
        List<Thread> senders = new ArrayList<>();
        for (int i = 0; i < 4; i++) {
            Thread t = new Thread(() -> {
                for (int k = 0; k < 1000; k++) {
                    inbox.add(new Message("add", 1));
                }
            });
            senders.add(t);
            t.start();
        }
        for (Thread t : senders) {
            t.join();
        }
        inbox.put(new Message("get", 0));
        System.out.println("total " + replies.take());
        inbox.put(new Message("stop", 0));
        worker.join();

        // A bounded queue refuses a message when it is full.
        BlockingQueue<String> small = new ArrayBlockingQueue<>(2);
        System.out.println(small.offer("a") + " " + small.offer("b") + " " + small.offer("c") + " " + small.remainingCapacity());

        // A SynchronousQueue has no buffer: offer fails unless a receiver waits.
        BlockingQueue<String> handoff = new SynchronousQueue<>();
        System.out.println(handoff.offer("x"));
    }
}
```

```output for=blocking-queue-inbox
total 4000
true true false 0
false
```

- Verdict: stdlib. A worker thread takes messages from a `LinkedBlockingQueue` and owns the total, so 4000 messages from four threads give 4000 with no lock. A `get` message gets its answer on a second queue. A full `ArrayBlockingQueue` refuses `offer`, and a `SynchronousQueue` refuses `offer` when no receiver waits.

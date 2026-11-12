```java run id=balking-pattern
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.atomic.AtomicInteger;

public class Main {
    static class Document {
        private final Runnable write; // the slow part of a save
        private boolean changed = false;
        private boolean saving = false;
        int saves = 0;

        Document(Runnable write) {
            this.write = write;
        }

        synchronized void edit() {
            changed = true;
        }

        // Saves the document. Returns false at once if a save cannot start now.
        boolean save() {
            synchronized (this) {
                if (saving || !changed) return false; // balk: the object is not in the right state
                saving = true;
                changed = false;
            }
            write.run();
            synchronized (this) {
                saving = false;
                saves++;
            }
            return true;
        }

        synchronized int saves() {
            return saves;
        }
    }

    public static void main(String[] args) throws InterruptedException {
        CountDownLatch started = new CountDownLatch(1);
        CountDownLatch release = new CountDownLatch(1);
        Document doc = new Document(() -> {
            started.countDown();
            try {
                release.await(); // the first save stays in progress until the other calls end
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        });
        doc.edit();
        Thread first = new Thread(doc::save);
        first.start();
        started.await();

        AtomicInteger calls = new AtomicInteger();
        AtomicInteger balked = new AtomicInteger();
        List<Thread> callers = new ArrayList<>();
        for (int i = 0; i < 7; i++) {
            callers.add(new Thread(() -> {
                calls.incrementAndGet();
                if (!doc.save()) balked.incrementAndGet();
            }));
        }
        for (Thread t : callers) t.start();
        for (Thread t : callers) t.join();
        release.countDown();
        first.join();

        System.out.println("calls during a save: " + calls.get());
        System.out.println("calls that balked: " + balked.get());
        System.out.println("saves run: " + doc.saves());
        System.out.println("save with no changes: " + (doc.save() ? "ran" : "balked"));
        doc.edit();
        System.out.println("save after an edit: " + (doc.save() ? "ran" : "balked"));
        System.out.println("saves run: " + doc.saves());
    }
}
```

```output
calls during a save: 7
calls that balked: 7
saves run: 1
save with no changes: balked
save after an edit: ran
saves run: 2
```

- Verdict: native. A `synchronized` block makes the test and the change of the `saving` flag one step.

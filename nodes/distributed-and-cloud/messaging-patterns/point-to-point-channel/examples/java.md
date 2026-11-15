```java run id=competing-workers
import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.concurrent.LinkedBlockingQueue;

public class Main {
    static final String STOP = "stop";

    public static void main(String[] args) throws InterruptedException {
        BlockingQueue<String> jobs = new LinkedBlockingQueue<>();
        ConcurrentLinkedQueue<String> done = new ConcurrentLinkedQueue<>();
        List<Thread> workers = new ArrayList<>();
        for (int i = 1; i <= 3; i++) {
            String name = "w" + i;
            Thread t = new Thread(() -> {
                try {
                    for (String job = jobs.take(); !job.equals(STOP); job = jobs.take()) {
                        done.add(job + "@" + name);
                    }
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
            });
            workers.add(t);
            t.start();
        }
        for (int n = 1; n <= 9; n++) {
            jobs.put("job-" + n);
        }
        for (int i = 0; i < workers.size(); i++) {
            jobs.put(STOP); // one stop message for each worker
        }
        for (Thread t : workers) {
            t.join();
        }

        List<String> names = new ArrayList<>();
        for (String d : done) {
            names.add(d.substring(0, d.indexOf('@')));
        }
        names.sort(null);
        System.out.println("handled: " + String.join(" ", names));
        System.out.println("each job once: " + (names.size() == 9 && new HashSet<>(names).size() == 9));
    }
}
```

```output
handled: job-1 job-2 job-3 job-4 job-5 job-6 job-7 job-8 job-9
each job once: true
```

- Verdict: stdlib. `BlockingQueue.take` removes one element and waits when the queue is empty, so each job goes to one thread.
- The stop message is a plain string here. A typed message, for example a sealed interface with a job record and a stop record, is safer when a job can have any text.
- A JMS or Jakarta Messaging client has the same shape: a `MessageConsumer` for each worker on one `Queue`, and the provider gives each message to one consumer.

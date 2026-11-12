```java run id=leader-followers
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.LinkedBlockingQueue;

public class Main {
    // Stands for a handle set that one thread at a time may wait on.
    static final LinkedBlockingQueue<Integer> handleSet = new LinkedBlockingQueue<>();

    static class ThreadPool {
        private boolean leaderPresent = false;
        private int waiting = 0;
        int mostWaiting = 0;
        int processed = 0;
        long sum = 0;

        void join() throws InterruptedException {
            while (true) {
                synchronized (this) { // follower role: wait for the turn to lead
                    while (leaderPresent) wait();
                    leaderPresent = true;
                    waiting++;
                    mostWaiting = Math.max(mostWaiting, waiting);
                }
                int event = handleSet.take(); // leader role: wait for an event
                promoteNewLeader();
                if (event < 0) return;
                Thread.sleep(1); // processing role: handle the event in this thread
                synchronized (this) {
                    processed++;
                    sum += event;
                }
            }
        }

        synchronized void promoteNewLeader() {
            waiting--;
            leaderPresent = false;
            notify();
        }
    }

    public static void main(String[] args) throws InterruptedException {
        for (int id = 0; id < 100; id++) handleSet.put(id);
        for (int i = 0; i < 4; i++) handleSet.put(-1); // one stop event per thread
        ThreadPool pool = new ThreadPool();
        List<Thread> threads = new ArrayList<>();
        for (int i = 0; i < 4; i++) {
            Thread t = new Thread(() -> {
                try {
                    pool.join();
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
            });
            t.start();
            threads.add(t);
        }
        for (Thread t : threads) t.join();
        System.out.println("events processed: " + pool.processed);
        System.out.println("sum of event ids: " + pool.sum);
        System.out.println("most threads waiting on the handle set at once: " + pool.mostWaiting);
    }
}
```

```output
events processed: 100
sum of event ids: 4950
most threads waiting on the handle set at once: 1
```

- Verdict: stdlib. The pool object's monitor is the synchronizer: `wait` holds the followers and `notify` promotes one of them.

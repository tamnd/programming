```java run id=active
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class Main {
    // The servant: only the scheduler thread calls it, so it has no lock.
    static class AccountServant {
        private int total = 0;
        int deposit(int amount) { total += amount; return total; }
        int balance() { return total; }
    }

    // The proxy: each method makes a request and returns a future at once.
    static class AccountProxy {
        private final AccountServant servant = new AccountServant();
        private final ExecutorService scheduler = Executors.newSingleThreadExecutor();
        final CountDownLatch gate = new CountDownLatch(1); // lets the demo hold the scheduler

        AccountProxy() {
            scheduler.execute(() -> {
                try {
                    gate.await();
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
            });
        }

        CompletableFuture<Integer> deposit(int amount) {
            return CompletableFuture.supplyAsync(() -> servant.deposit(amount), scheduler);
        }

        CompletableFuture<Integer> balance() {
            return CompletableFuture.supplyAsync(servant::balance, scheduler);
        }

        void stop() { scheduler.shutdown(); }
    }

    public static void main(String[] args) throws Exception {
        AccountProxy account = new AccountProxy();
        List<List<CompletableFuture<Integer>>> perClient = new ArrayList<>();
        List<Thread> clients = new ArrayList<>();
        for (int c = 0; c < 4; c++) {
            List<CompletableFuture<Integer>> mine = new ArrayList<>();
            perClient.add(mine);
            Thread t = new Thread(() -> {
                for (int i = 0; i < 250; i++) mine.add(account.deposit(1)); // returns at once
            });
            clients.add(t);
            t.start();
        }
        for (Thread t : clients) t.join();
        int queued = 0, done = 0;
        for (List<CompletableFuture<Integer>> mine : perClient) {
            for (CompletableFuture<Integer> f : mine) {
                queued++;
                if (f.isDone()) done++;
            }
        }
        System.out.println("deposits queued: " + queued);
        System.out.println("deposits done before the scheduler starts: " + done);
        account.gate.countDown();
        System.out.println("balance after 1000 deposits: " + account.balance().get());
        account.stop();
    }
}
```

```output
deposits queued: 1000
deposits done before the scheduler starts: 0
balance after 1000 deposits: 1000
```

- Verdict: stdlib. A single-thread `ExecutorService` is the scheduler with its queue, and `CompletableFuture` is the future.

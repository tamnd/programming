```java run id=mailbox
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.Executors;
import java.util.concurrent.LinkedBlockingQueue;

public class Main {
    sealed interface Msg permits Deposit, Withdraw, Balance, Stop {}
    record Deposit(int amount) implements Msg {}
    record Withdraw(int amount) implements Msg {}
    record Balance(CompletableFuture<Integer> reply) implements Msg {}
    record Stop() implements Msg {}

    static class Account implements Runnable {
        final BlockingQueue<Msg> mailbox = new LinkedBlockingQueue<>();
        private int balance; // only the actor's thread uses this field

        public void run() {
            try {
                while (true) {
                    switch (mailbox.take()) {
                        case Deposit d -> balance += d.amount();
                        case Withdraw w -> {
                            if (w.amount() <= balance) balance -= w.amount();
                        }
                        case Balance b -> b.reply().complete(balance);
                        case Stop s -> {
                            return;
                        }
                    }
                }
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        }
    }

    public static void main(String[] args) throws Exception {
        Account account = new Account();
        Thread actor = Thread.ofVirtual().start(account);
        try (var clients = Executors.newVirtualThreadPerTaskExecutor()) {
            for (int i = 0; i < 100; i++) {
                clients.submit(() -> account.mailbox.add(new Deposit(10)));
            }
        }
        account.mailbox.add(new Withdraw(1));
        var reply = new CompletableFuture<Integer>();
        account.mailbox.add(new Balance(reply));
        System.out.println("balance " + reply.get());
        account.mailbox.add(new Stop());
        actor.join();
    }
}
```

```output for=mailbox
balance 999
```

- Verdict: library. The JDK has the parts: threads, `BlockingQueue` for a mailbox, records and sealed interfaces for messages, and `CompletableFuture` for replies. Akka and its fork Apache Pekko give full actor systems with supervision and distribution.
- One hundred tasks deposit at the same time, but no lock protects `balance`. The mailbox puts the messages in one line, and one thread handles them.

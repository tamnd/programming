```java run id=counter
import java.util.concurrent.locks.ReentrantLock;

public class Main {
    static class Account {
        private int balance;

        synchronized void deposit(int amount) {
            balance += amount;
        }

        synchronized void depositTwice(int amount) {
            deposit(amount);
            deposit(amount);
        }

        synchronized int balance() {
            return balance;
        }
    }

    public static void main(String[] args) throws InterruptedException {
        Account account = new Account();
        Thread[] workers = new Thread[4];
        for (int i = 0; i < workers.length; i++) {
            workers[i] = new Thread(() -> {
                for (int k = 0; k < 10_000; k++) {
                    account.depositTwice(1);
                }
            });
            workers[i].start();
        }
        for (Thread worker : workers) {
            worker.join();
        }
        System.out.println("balance: " + account.balance());

        ReentrantLock lock = new ReentrantLock();
        lock.lock();
        lock.lock();
        System.out.println("hold count: " + lock.getHoldCount());
        lock.unlock();
        lock.unlock();
        System.out.println("locked: " + lock.isLocked());
    }
}
```

```output
balance: 80000
hold count: 2
locked: false
```

- Verdict: native. Each object has a monitor. A `synchronized` method locks the monitor of `this`, and the lock is released when the method returns or throws.
- Monitors are reentrant. `depositTwice` holds the monitor and calls `deposit`, which locks the same monitor again.
- `java.util.concurrent.locks.ReentrantLock` adds `tryLock`, timeouts and an optional fair mode. Call `lock()`, then `try`, and `unlock()` in `finally`. The owner must unlock once for each lock.
- `ReentrantReadWriteLock` gives a read lock and a write lock. A reader cannot upgrade to the write lock, but a writer can downgrade to the read lock.

A thread that calls `unlock` without the lock gets an exception.

```java fails expect=exception:IllegalMonitorStateException id=not-owner
import java.util.concurrent.locks.ReentrantLock;

public class Main {
    public static void main(String[] args) {
        ReentrantLock lock = new ReentrantLock();
        lock.unlock();
    }
}
```

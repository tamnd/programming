```java run id=stop-a-thread
public class Main {
    public static void main(String[] args) throws InterruptedException {
        // A thread that sleeps: interrupt() wakes it with an InterruptedException.
        Thread sleeper = new Thread(() -> {
            try {
                Thread.sleep(10_000);
            } catch (InterruptedException e) {
                System.out.println("sleeper: interrupted, cleaning up");
                System.out.println("interrupt status after the exception: "
                        + Thread.currentThread().isInterrupted());
            }
        });
        sleeper.start();
        Thread.sleep(100);
        long start = System.nanoTime();
        sleeper.interrupt();
        sleeper.join();
        System.out.println("sleeper stopped in under 1 s: "
                + (System.nanoTime() - start < 1_000_000_000L));

        // A thread that only calculates must check its interrupt status itself.
        Thread counter = new Thread(() -> {
            long n = 0;
            while (!Thread.currentThread().isInterrupted()) {
                n++;
            }
            System.out.println("counter: saw the interrupt status, stopping");
        });
        counter.start();
        Thread.sleep(100);
        counter.interrupt();
        counter.join();
        System.out.println("both threads alive: " + (sleeper.isAlive() || counter.isAlive()));
    }
}
```

```output for=stop-a-thread
sleeper: interrupted, cleaning up
interrupt status after the exception: false
sleeper stopped in under 1 s: true
counter: saw the interrupt status, stopping
both threads alive: false
```

- Verdict: stdlib. `Thread.interrupt` asks a thread to stop. The tutorial calls an interrupt "an indication to a thread that it should stop what it is doing and do something else".
- The sleeper is blocked in `sleep`, so "its interrupt status will be cleared and it will receive an InterruptedException". The second line prints `false` for this reason.
- The counter never calls a method that throws `InterruptedException`, so it checks `isInterrupted()` in its loop. Without that check, `interrupt()` would only set the status, and the loop would never end.
- `Thread.stop` is not an alternative. It is deprecated for removal, and the documentation says: "It was inherently unsafe."

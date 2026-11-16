```java run id=daemon
import java.time.Duration;

public class Main {
    public static void main(String[] args) throws InterruptedException {
        Thread worker = Thread.ofPlatform().start(() -> System.out.println("worker ran"));
        worker.join();
        System.out.println("after join, alive: " + worker.isAlive());

        Thread sleeper = Thread.ofPlatform().daemon(true).start(() -> {
            try {
                Thread.sleep(Duration.ofHours(1));
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        });
        boolean ended = sleeper.join(Duration.ofMillis(50));
        System.out.println("daemon ended within 50 ms: " + ended);
        System.out.println("main returns; the JVM does not wait for the daemon thread");
    }
}
```

```output for=daemon
worker ran
after join, alive: false
daemon ended within 50 ms: false
main returns; the JVM does not wait for the daemon thread
```

- Verdict: stdlib. `Thread.join()` waits without a limit, and `join(Duration)` waits for a limited time and returns `true` if the thread has ended.
- Java has no detach. A daemon thread does not delay the shutdown of the JVM, so it acts like a detached thread at exit. The flag must be set before the thread starts; here the builder sets it.
- The sleeper thread is still running when `main` returns. The JVM exits because only a daemon thread is left.
- Virtual threads are always daemon threads.

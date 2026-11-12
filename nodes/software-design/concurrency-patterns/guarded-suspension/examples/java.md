```java run id=guarded-suspension
import java.util.ArrayList;
import java.util.List;

public class Main {
    static class Tank {
        private int level = 0;
        int lowest = 0; // the lowest level seen after a take
        int filled = 0;
        int taken = 0;
        int served = 0;

        synchronized void take(int amount) throws InterruptedException {
            // Guarded suspension: wait in a loop until the guard is true.
            while (level < amount) {
                wait();
            }
            level -= amount;
            lowest = Math.min(lowest, level);
            taken += amount;
            served++;
        }

        synchronized void fill(int amount) {
            level += amount;
            filled += amount;
            notifyAll(); // each waiting taker checks its guard again
        }

        synchronized int level() {
            return level;
        }
    }

    public static void main(String[] args) throws InterruptedException {
        Tank tank = new Tank();
        int[] amounts = {5, 3, 8};
        List<Thread> takers = new ArrayList<>();
        for (int a : amounts) {
            Thread t = new Thread(() -> {
                try {
                    tank.take(a);
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
            });
            takers.add(t);
            t.start();
        }
        for (int i = 0; i < 10; i++) tank.fill(2);
        for (Thread t : takers) t.join();

        synchronized (tank) {
            System.out.println("units filled: " + tank.filled);
            System.out.println("units taken: " + tank.taken);
            System.out.println("takers served: " + tank.served);
            System.out.println("level never below 0: " + (tank.lowest >= 0 ? "True" : "False"));
        }
        System.out.println("units left: " + tank.level());
    }
}
```

```output
units filled: 20
units taken: 16
takers served: 3
level never below 0: True
units left: 4
```

- Verdict: native. `synchronized` methods with `wait()` in a `while` loop and `notifyAll()`.

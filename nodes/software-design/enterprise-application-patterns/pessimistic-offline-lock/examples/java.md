```java run id=lock-table-with-timeout
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.TreeMap;

public class Main {
    static class LockException extends RuntimeException {
        LockException(String message) { super(message); }
    }

    record Lock(String owner, int taken) {}

    // The lock manager keeps the locks in a table, so that they last across requests.
    // In a real application, the table is in the database and the resource is its primary key.
    static class LockManager {
        static final int TIMEOUT = 30; // minutes: a lock that is older than this has been abandoned
        private final Map<String, Lock> table = new TreeMap<>();

        synchronized void acquire(String resource, String owner, int now) {
            table.computeIfPresent(resource, (r, lock) -> lock.taken() <= now - TIMEOUT ? null : lock);
            Lock holder = table.putIfAbsent(resource, new Lock(owner, now));
            if (holder != null && !holder.owner().equals(owner)) {
                throw new LockException(resource + " is locked by " + holder.owner());
            }
        }

        synchronized void releaseAll(String owner) {
            table.values().removeIf(lock -> lock.owner().equals(owner));
        }

        synchronized List<String> held() {
            List<String> out = new ArrayList<>();
            table.forEach((r, lock) -> out.add(r + " by " + lock.owner()));
            return out;
        }
    }

    static final LockManager locks = new LockManager();

    static String clock(int minute) { return String.format("10:%02d", minute); }

    static void openForEdit(int minute, String who, String resource) {
        try {
            locks.acquire(resource, who, minute);
            System.out.println(clock(minute) + " " + who + " opens " + resource + " for edit: lock granted");
        } catch (LockException e) {
            System.out.println(clock(minute) + " " + who + " opens " + resource + " for edit: refused: " + e.getMessage());
        }
    }

    static void close(int minute, String who) {
        locks.releaseAll(who);
        System.out.println(clock(minute) + " " + who + " saves and closes: locks released");
    }

    public static void main(String[] args) {
        openForEdit(0, "ann", "customer 1");
        openForEdit(5, "bob", "customer 1");
        close(12, "ann");
        openForEdit(15, "bob", "customer 1");
        openForEdit(20, "bob", "order 9");
        // Bob closes the browser and never comes back.
        openForEdit(40, "carol", "customer 1");
        openForEdit(50, "carol", "customer 1");
        System.out.println("locks held: " + String.join(", ", locks.held()));
    }
}
```

```output
10:00 ann opens customer 1 for edit: lock granted
10:05 bob opens customer 1 for edit: refused: customer 1 is locked by ann
10:12 ann saves and closes: locks released
10:15 bob opens customer 1 for edit: lock granted
10:20 bob opens order 9 for edit: lock granted
10:40 carol opens customer 1 for edit: refused: customer 1 is locked by bob
10:50 carol opens customer 1 for edit: lock granted
locks held: customer 1 by carol, order 9 by bob
```

- Verdict: idiom. The methods of the lock manager are `synchronized`, and `putIfAbsent` checks and takes the lock in one call. `computeIfPresent` removes a lock that is older than the timeout.
- The resource names are keys of a `TreeMap`, so `held` lists the locks in a stable order.

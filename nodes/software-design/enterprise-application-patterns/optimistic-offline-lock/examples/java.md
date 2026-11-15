```java run id=two-sessions-one-customer
import java.util.HashMap;
import java.util.Map;

public class Main {
    static class ConflictException extends RuntimeException {
        ConflictException(String message) { super(message); }
    }

    record Row(String name, int credit, int version) {}

    // A session's copy of the record, with the version that it read.
    static class Customer {
        final int id;
        String name;
        int credit;
        int version;
        Customer(int id, Row row) { this.id = id; name = row.name(); credit = row.credit(); version = row.version(); }
        @Override public String toString() { return name + ", credit " + credit + ", version " + version; }
    }

    // The table, kept in memory.
    static final Map<Integer, Row> table = new HashMap<>(Map.of(1, new Row("Acme Ltd", 500, 1)));

    static Customer load(int id) { return new Customer(id, table.get(id)); }

    // The check and the update are one step, like UPDATE ... WHERE version = ? in SQL.
    static synchronized void save(Customer c) {
        Row row = table.get(c.id);
        if (row.version() != c.version) throw new ConflictException("customer " + c.id + " was changed by another session");
        table.put(c.id, new Row(c.name, c.credit, row.version() + 1));
        c.version++;
    }

    static boolean trySave(String who, Customer c, String what) {
        try {
            save(c);
            System.out.println(who + " saves " + what + ": ok, version " + c.version);
            return true;
        } catch (ConflictException e) {
            System.out.println(who + " saves " + what + ": conflict: " + e.getMessage());
            return false;
        }
    }

    public static void main(String[] args) {
        Customer ann = load(1);
        System.out.println("ann loads customer 1: " + ann);
        Customer bob = load(1);
        System.out.println("bob loads customer 1: " + bob);

        bob.credit = 800;
        trySave("bob", bob, "credit 800");

        ann.name = "Acme Group";
        if (!trySave("ann", ann, "name Acme Group")) {
            ann = load(1); // the session reloads, shows the new data, and the user does the edit again
            System.out.println("ann reloads: " + ann);
            ann.name = "Acme Group";
            trySave("ann", ann, "name Acme Group");
        }

        System.out.println("final: " + load(1));
    }
}
```

```output
ann loads customer 1: Acme Ltd, credit 500, version 1
bob loads customer 1: Acme Ltd, credit 500, version 1
bob saves credit 800: ok, version 2
ann saves name Acme Group: conflict: customer 1 was changed by another session
ann reloads: Acme Ltd, credit 800, version 2
ann saves name Acme Group: ok, version 3
final: Acme Group, credit 800, version 3
```

- Verdict: idiom. `save` is `synchronized`, so the check of the version and the write are one step. In Jakarta Persistence, a field with the `@Version` annotation gives the same check, and the provider throws an `OptimisticLockException` on a conflict.
- `ConflictException` is unchecked. The session catches it, reloads the record and lets the user do the edit again.

```java run id=talk-to-the-worker
import java.util.ArrayList;
import java.util.List;

public class Main {
    record Entry(int day, long cents) {}

    static class Ledger {
        private final List<Entry> entries = new ArrayList<>();
        void add(int day, long cents) { entries.add(new Entry(day, cents)); }
        long balance() { return entries.stream().mapToLong(Entry::cents).sum(); }
        List<Entry> since(int day) { return entries.stream().filter(e -> e.day() >= day).toList(); }
    }

    // After: a getter for the delegate in place of one forwarding method per feature.
    static class Account {
        private final String owner;
        private final Ledger ledger = new Ledger();
        Account(String owner) { this.owner = owner; }
        String owner() { return owner; }
        Ledger getLedger() { return ledger; }
    }

    public static void main(String[] args) {
        Account a = new Account("Ana");
        a.getLedger().add(1, 500);
        a.getLedger().add(3, -120);
        a.getLedger().add(9, 40);
        if (a.getLedger().balance() != 420) throw new AssertionError("balance");
        if (a.getLedger().since(3).size() != 2) throw new AssertionError("since");
        System.out.println("remove middle man: tests pass");
    }
}
```

```output
remove middle man: tests pass
```

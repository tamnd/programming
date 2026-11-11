```java run id=private-fields
import java.util.ArrayList;
import java.util.List;

final class Account {  // final: no subclass can change the rules
    private long balance;  // only code in Account can name these fields
    private final List<Long> history = new ArrayList<>();

    void deposit(long amount) {
        if (amount <= 0) throw new IllegalArgumentException("deposit must be positive");
        history.add(amount);
        balance += amount;
    }

    void withdraw(long amount) {
        if (amount > balance) throw new IllegalStateException("insufficient funds");
        history.add(-amount);
        balance -= amount;
    }

    long balance() { return balance; }

    List<Long> history() { return List.copyOf(history); }  // an unmodifiable copy

    boolean sameBalance(Account other) {
        return balance == other.balance;  // access is per class, not per object
    }
}

public class Main {
    public static void main(String[] args) {
        Account a = new Account();
        a.deposit(100);
        a.withdraw(30);
        System.out.println(a.balance() + " " + a.history());
        try {
            a.history().add(1_000_000L);
        } catch (UnsupportedOperationException e) {
            System.out.println("history is read-only");
        }
        try {
            a.withdraw(500);
        } catch (IllegalStateException e) {
            System.out.println(e.getMessage());
        }
        Account b = new Account();
        b.deposit(70);
        System.out.println(a.sameBalance(b));
    }
}
```

```output for=private-fields
70 [100, -30]
history is read-only
insufficient funds
true
```

```java fails expect=compile id=private-access
class Account {
    private long balance;

    long balance() { return balance; }
}

public class Main {
    public static void main(String[] args) {
        Account a = new Account();
        a.balance = 1_000_000;  // balance has private access in Account
        System.out.println(a.balance());
    }
}
```

```java wrong id=mutable-field
import java.util.Date;

final class Meeting {
    private final Date start;  // final reference, mutable object

    Meeting(Date start) { this.start = start; }  // keeps the caller's object

    Date start() { return start; }
}

public class Main {
    public static void main(String[] args) {
        Date d = new Date(0);
        Meeting m = new Meeting(d);
        d.setTime(86_400_000L);           // the caller changes its own Date ...
        m.start().setTime(172_800_000L);  // ... or the one the getter returns
        System.out.println(m.start().getTime());
    }
}
```

```output for=mutable-field
172800000
```

- Verdict: native. Java has four levels of access for members: `private` for the class, no modifier for the package, `protected` for the package and subclasses, and `public`. Modules add a level above packages, because a module exports only the packages that it lists. Access is checked per class, not per object, so `sameBalance` can read the balance of another account. `List.copyOf` returns an unmodifiable list, so a getter can expose data without exposing the field.
- The failing block assigns a private field from another class, which the compiler rejects. The wrong block shows that `private final` protects only the reference: the constructor keeps the caller's `Date`, and the getter returns the same object, so outside code changes the meeting through both. Copy mutable values when they come in and when they go out, or use immutable types such as `java.time.Instant`.

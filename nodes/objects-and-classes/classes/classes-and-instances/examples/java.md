```java run id=fields-statics-and-records
import java.util.Objects;

final class Account {
    static final double RATE = 0.02; // static: one value for the class
    private static int opened = 0;

    private final String owner; // instance fields: one set per object
    private long balance;

    Account(String owner, long balance) {
        this.owner = Objects.requireNonNull(owner);
        this.balance = balance;
        opened++;
    }

    Account deposit(long amount) {
        balance += amount;
        return this;
    }

    double interest() { return balance * RATE; }

    static int opened() { return opened; } // a static method has no this

    @Override public String toString() { return "Account(" + owner + ", " + balance + ")"; }
}

// A record: a class whose fields, constructor, accessors, equals and toString come from one line.
record Point(int x, int y) {}

public class Main {
    public static void main(String[] args) {
        var a = new Account("ana", 100);
        var b = new Account("bo", 0);
        a.deposit(50);
        System.out.println(a + " " + b + " " + Account.opened());
        System.out.println(a.interest());
        System.out.println(a.getClass().getSimpleName() + " " + (a.getClass() == Account.class));
        System.out.println(new Point(1, 2) + " " + new Point(1, 2).equals(new Point(1, 2)));
    }
}
```

```output for=fields-statics-and-records
Account(ana, 150) Account(bo, 0) 2
3.0
Account true
Point[x=1, y=2] true
```

- Verdict: native. Every method and field in Java belongs to a class. Instance fields belong to each object and static fields to the class, and a static method has no `this`. Fields are usually `private` and reached through methods, and `final` fields cannot change after construction. Each object knows its class at run time, through `getClass`, and `Account.class` is the same `Class` object. A record declares a class for plain data in one line, with the constructor, accessors, `equals`, `hashCode` and `toString` generated from its components.

```java run id=accounts
public class Main {
    abstract static class Account {
        private int balance;             // private: subclasses go through deposit
        protected final String owner;    // protected: subclasses can read it

        Account(String owner) { this.owner = owner; }

        void deposit(int n) { balance += n; }
        int balance() { return balance; }
        abstract String kind();          // each subclass must supply it

        String describe() { return kind() + " of " + owner + ": " + balance; }
    }

    static class Savings extends Account {
        Savings(String owner) { super(owner); }
        String kind() { return "savings"; }
        void addInterest() { deposit(balance() / 50); }
    }

    static final class Checking extends Account { // final: no class can extend it
        private final int overdraft;

        Checking(String owner, int overdraft) {
            super(owner);                // the base constructor runs first
            this.overdraft = overdraft;
        }

        String kind() { return "checking"; }

        @Override
        String describe() { return super.describe() + " (overdraft " + overdraft + ")"; }
    }

    public static void main(String[] args) {
        Savings s = new Savings("ana");
        s.deposit(1000);
        s.addInterest();
        Account c = new Checking("bo", 100);
        c.deposit(50);
        for (Account a : new Account[] { s, c })
            System.out.println(a.describe());
        System.out.println(c.getClass().getSuperclass().getSimpleName());
    }
}
```

```output for=accounts
savings of ana: 1020
checking of bo: 50 (overdraft 100)
Account
```

```java wrong id=fragile-base
import java.util.Collection;
import java.util.HashSet;
import java.util.List;

public class Main {
    // A set that counts how many elements were ever added to it.
    static class CountingSet<E> extends HashSet<E> {
        int added = 0;

        @Override
        public boolean add(E e) {
            added++;
            return super.add(e);
        }

        @Override
        public boolean addAll(Collection<? extends E> c) {
            added += c.size();
            return super.addAll(c); // the inherited addAll calls add for each element
        }
    }

    public static void main(String[] args) {
        CountingSet<String> s = new CountingSet<>();
        s.addAll(List.of("a", "b", "c"));
        System.out.println(s.added);
    }
}
```

```output for=fragile-base
6
```

- Verdict: native. A class `extends` one class and inherits its fields and methods, and every class but `Object` has exactly one superclass. An `abstract` class can declare methods that each subclass must supply, `protected` members are visible to subclasses, and a `final` class cannot be extended. A constructor must call a constructor of the superclass first, with `super(...)` or implicitly. Interfaces give the multiple inheritance of types and of default methods, not of fields.
- The wrong block is the fragile base class problem. `CountingSet` overrides `add` and `addAll`, but the inherited `addAll` of `HashSet` calls `add` for each element, so three elements count six times. The subclass depends on how its base calls its own methods, which the documentation of `HashSet` does not promise. Wrap a set in a new class and forward the calls instead of extending it.

```java run id=account
public class Main {
    static final class Account {
        private int balance;

        Account(int balance) {
            if (balance < 0) {
                throw new IllegalArgumentException("negative opening balance " + balance);
            }
            this.balance = balance;
        }

        void withdraw(int amount) {
            if (amount <= 0 || amount > balance) {
                throw new IllegalArgumentException("cannot withdraw " + amount + " from " + balance);
            }
            int old = balance;
            balance -= amount;
            assert balance == old - amount : "postcondition";
            assert balance >= 0 : "invariant: balance is " + balance;
        }
    }

    public static void main(String[] args) {
        boolean enabled = false;
        assert enabled = true;
        System.out.println("assertions enabled: " + enabled);

        Account account = new Account(100);
        account.withdraw(30);
        System.out.println("balance " + account.balance);
        try {
            account.withdraw(500);
        } catch (IllegalArgumentException e) {
            System.out.println("rejected: " + e.getMessage());
        }
    }
}
```

```output
assertions enabled: false
balance 70
rejected: cannot withdraw 500 from 70
```

- Verdict: native. `assert condition : message;` throws `AssertionError` with the message when assertions are enabled and the condition is false.
- Assertions are disabled by default. The option `-ea` of the `java` launcher enables them. A disabled `assert` does not evaluate its expressions. The line `assert enabled = true;` uses this to report the state.
- The JLS says that assertions should not check the arguments of public methods, because the check must hold whether assertions are enabled or not. The example checks the arguments with `if` and `IllegalArgumentException`, and uses `assert` for the postcondition and the invariant.

The launcher option `-ea` is one way to enable assertions. A program can also set the default assertion status of a class loader. The setting applies to the classes that the loader initializes after the call, here `Ledger`. A failed `assert` in `Ledger` then throws `AssertionError`.

```java fails expect=exception:AssertionError id=enabled
public class Main {
    public static void main(String[] args) {
        ClassLoader.getSystemClassLoader().setDefaultAssertionStatus(true);
        Ledger.withdraw(500);
        System.out.println("balance " + Ledger.balance);
    }
}

class Ledger {
    static int balance = 100;

    static void withdraw(int amount) {
        balance -= amount;
        assert balance >= 0 : "invariant: balance is " + balance;
    }
}
```

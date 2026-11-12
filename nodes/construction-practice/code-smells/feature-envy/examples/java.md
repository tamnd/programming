```java run id=move-to-the-data
public class Main {
    static final class Account {
        private final long balance;
        private final long overdraftLimit;
        private final boolean frozen;

        Account(long balance, long overdraftLimit, boolean frozen) {
            this.balance = balance;
            this.overdraftLimit = overdraftLimit;
            this.frozen = frozen;
        }

        // Before the move, other classes needed these three getters.
        long getBalance() { return balance; }
        long getOverdraftLimit() { return overdraftLimit; }
        boolean isFrozen() { return frozen; }

        // After: the rule lives with the data it reads.
        boolean canWithdraw(long amount) {
            return !frozen && balance + overdraftLimit >= amount;
        }
    }

    // Before: the cash machine class asks the account for its data.
    static boolean canWithdrawBefore(Account account, long amount) {
        if (account.isFrozen()) return false;
        return account.getBalance() + account.getOverdraftLimit() >= amount;
    }

    static void check(boolean ok, String what) {
        if (!ok) throw new AssertionError(what);
    }

    public static void main(String[] args) {
        Object[][] cases = {
            {new Account(100, 50, false), 150L, true},
            {new Account(100, 50, false), 151L, false},
            {new Account(100, 0, true), 10L, false},
        };
        for (Object[] c : cases) {
            Account account = (Account) c[0];
            long amount = (Long) c[1];
            boolean expected = (Boolean) c[2];
            check(canWithdrawBefore(account, amount) == expected, "before " + amount);
            check(account.canWithdraw(amount) == expected, "after " + amount);
        }
        System.out.println("canWithdraw: tests pass");
    }
}
```

```output
canWithdraw: tests pass
```

- `canWithdrawBefore` makes three getter calls on one object and no call on its own class. That count is the sign of the smell.
- After the move, `canWithdraw` reads the private fields directly. If no other class calls the getters, they can be removed, and the fields are then hidden behind the one method.

```java run id=stop-the-trade
import java.util.ArrayList;
import java.util.List;

public class Main {
    // Before: the subclass reads its parent's protected list.
    static class AccountBefore {
        protected final List<Integer> payments = new ArrayList<>();
        protected int charged;
        void pay(int cents) { payments.add(cents); }
        void charge(int cents) { charged += cents; }
        int due() { return charged - payments.stream().mapToInt(Integer::intValue).sum(); }
    }

    static class RewardsAccountBefore extends AccountBefore {
        int points() { return payments.stream().mapToInt(c -> c / 100).sum(); }
    }

    // After: the account keeps its data private and tells a delegate about each payment.
    interface Rewards {
        void onPayment(int cents);
        int points();
    }

    static final class PointPerDollar implements Rewards {
        private int points;
        public void onPayment(int cents) { points += cents / 100; }
        public int points() { return points; }
    }

    enum NoRewards implements Rewards {
        INSTANCE;
        public void onPayment(int cents) {}
        public int points() { return 0; }
    }

    static final class Account {
        private final List<Integer> payments = new ArrayList<>();
        private int charged;
        private final Rewards rewards;

        Account(Rewards rewards) { this.rewards = rewards; }
        Account() { this(NoRewards.INSTANCE); }

        void pay(int cents) {
            payments.add(cents);
            rewards.onPayment(cents);
        }
        void charge(int cents) { charged += cents; }
        int due() { return charged - payments.stream().mapToInt(Integer::intValue).sum(); }
        int points() { return rewards.points(); }
    }

    static void check(boolean ok, String what) {
        if (!ok) throw new AssertionError(what);
    }

    public static void main(String[] args) {
        RewardsAccountBefore before = new RewardsAccountBefore();
        Account after = new Account(new PointPerDollar());
        Account plain = new Account();
        for (int c : List.of(1500, 250, 999)) {
            before.pay(c);
            after.pay(c);
            plain.pay(c);
        }
        before.charge(4000);
        after.charge(4000);
        plain.charge(4000);
        check(after.due() == before.due() && after.due() == 1251, "due");
        check(after.points() == before.points() && after.points() == 26, "points");
        check(plain.points() == 0 && plain.due() == 1251, "plain");
        System.out.println("invoice: tests pass");
    }
}
```

```output
invoice: tests pass
```

- In the first version `payments` is `protected`, so the subclass depends on the parent's choice of a `List<Integer>`. A change to that field breaks the subclass.
- In the second version the fields are `private` and the class is `final`. The rewards rule gets each payment through `onPayment`, which is the only thing it knows about the account.
- This is Replace Subclass with Delegate. An account can also change its rewards rule at run time by taking a different delegate, which a subclass cannot do.

```java run id=name-the-choice
public class Main {
    record Booking(String customer, String seat, int fee) {}

    // Before: a flag argument chooses the operation.
    static Booking bookBefore(String customer, boolean isPremium) {
        if (isPremium) {
            return new Booking(customer, "front", 40);
        }
        return new Booking(customer, "general", 0);
    }

    // After, the callers pass literals: a method for each operation.
    static Booking premiumBook(String customer) { return new Booking(customer, "front", 40); }
    static Booking regularBook(String customer) { return new Booking(customer, "general", 0); }

    // After, the callers pass a value they read: an enum in place of the boolean.
    enum Tier { PREMIUM, REGULAR }

    static Booking book(String customer, Tier tier) {
        return switch (tier) {
            case PREMIUM -> premiumBook(customer);
            case REGULAR -> regularBook(customer);
        };
    }

    public static void main(String[] args) {
        check(premiumBook("ada").equals(bookBefore("ada", true)), "premium");
        check(regularBook("ada").equals(bookBefore("ada", false)), "regular");
        check(book("ada", Tier.valueOf("PREMIUM")).equals(bookBefore("ada", true)), "tier");
        boolean refused = false;
        try {
            Tier.valueOf("GOLD");
        } catch (IllegalArgumentException e) {
            refused = true;
        }
        check(refused, "an unknown tier should be refused");
        System.out.println("booking: tests pass");
    }

    static void check(boolean ok, String what) {
        if (!ok) {
            throw new AssertionError(what);
        }
    }
}
```

```output
booking: tests pass
```

The `switch` over `Tier` has a case for each constant and no `default`, so if a third tier is added, the switch expression no longer compiles until it has a case for it.

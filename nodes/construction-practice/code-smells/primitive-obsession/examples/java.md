```java run id=money-type
import java.util.List;

public class Main {
    record Line(long cents, String currency) {}

    // Before: money is a number of cents and a separate string.
    static long totalBefore(List<Line> lines) {
        return lines.stream().mapToLong(Line::cents).sum();
    }

    static String formatBefore(long cents, String currency) {
        return String.format("%d.%02d %s", cents / 100, cents % 100, currency);
    }

    // After: Money holds the amount, the currency and the rules.
    record Money(long cents, String currency) {
        Money {
            if (currency == null || !currency.matches("[A-Z]{3}")) {
                throw new IllegalArgumentException("currency code " + currency);
            }
        }

        Money plus(Money other) {
            if (!other.currency.equals(currency)) {
                throw new IllegalArgumentException("cannot add " + other.currency + " to " + currency);
            }
            return new Money(Math.addExact(cents, other.cents), currency);
        }

        @Override
        public String toString() {
            return String.format("%d.%02d %s", cents / 100, cents % 100, currency);
        }
    }

    static Money total(List<Money> prices) {
        return prices.stream().reduce(Money::plus).orElseThrow();
    }

    static void check(boolean ok, String what) {
        if (!ok) throw new AssertionError(what);
    }

    static boolean rejects(Runnable r) {
        try {
            r.run();
            return false;
        } catch (IllegalArgumentException e) {
            return true;
        }
    }

    public static void main(String[] args) {
        List<Line> lines = List.of(new Line(1250, "EUR"), new Line(800, "EUR"), new Line(500, "USD"));
        check(formatBefore(totalBefore(lines), "EUR").equals("25.50 EUR"), "before"); // 5 dollars as 5 euros
        System.out.println("before: the total mixes two currencies");

        List<Money> euros = List.of(new Money(1250, "EUR"), new Money(800, "EUR"));
        check(total(euros).toString().equals("20.50 EUR"), "total");
        check(rejects(() -> total(List.of(euros.get(0), new Money(500, "USD")))), "mixed currencies");
        check(rejects(() -> new Money(100, "euro")), "bad code");
        check(new Money(100, "EUR").equals(new Money(100, "EUR")), "equal by value");
        System.out.println("after: tests pass");
    }
}
```

```output
before: the total mixes two currencies
after: tests pass
```

- The `long` type of `cents` already rules out a fraction of a cent, so the record needs no check for it. `Math.addExact` throws on overflow in place of a silent wrap.
- The record gets `equals` and `hashCode` from its two components, so two amounts with the same cents and currency are equal.

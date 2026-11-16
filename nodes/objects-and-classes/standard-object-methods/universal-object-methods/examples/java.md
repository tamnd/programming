```java run id=override-equals-hashcode-tostring
import java.util.HashMap;
import java.util.Map;
import java.util.Objects;

public class Main {
    static final class Money {
        private final long cents;
        private final String currency;

        Money(long cents, String currency) {
            this.cents = cents;
            this.currency = Objects.requireNonNull(currency);
        }

        @Override
        public boolean equals(Object o) {
            if (this == o) return true;
            if (!(o instanceof Money m)) return false;
            return cents == m.cents && currency.equals(m.currency);
        }

        @Override
        public int hashCode() {
            return Objects.hash(cents, currency);
        }

        @Override
        public String toString() {
            return String.format("%d.%02d %s", cents / 100, cents % 100, currency);
        }
    }

    public static void main(String[] args) {
        Money a = new Money(1250, "EUR"), b = new Money(1250, "EUR");
        System.out.println(a.equals(b) + " " + (a.hashCode() == b.hashCode()) + " " + a.equals(null));
        Map<Money, String> names = new HashMap<>();
        names.put(a, "lunch");
        System.out.println(names.get(b) + " " + a);
    }
}
```

```output for=override-equals-hashcode-tostring
true true false
lunch 12.50 EUR
```

- Verdict: native. `java.lang.Object` declares `equals`, `hashCode` and `toString`, and every class inherits them.
- `equals` takes an `Object`, so it must check the type first; the `instanceof` pattern does the check and the cast in one step. `a.equals(null)` must return `false`.
- `Objects.hash` builds a hash code from the same fields that `equals` compares, so a `HashMap` finds the value with a second object that is equal to the key.

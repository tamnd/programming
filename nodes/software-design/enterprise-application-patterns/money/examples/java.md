```java run id=money-allocate
import java.util.ArrayList;
import java.util.Currency;
import java.util.List;
import java.util.stream.Collectors;

public class Main {
    // units: in the minor unit of the currency, such as cents. A record gives equals and hashCode.
    record Money(long units, Currency currency) {
        static Money of(long units, String code) {
            return new Money(units, Currency.getInstance(code));
        }

        Money plus(Money other) {
            if (!currency.equals(other.currency)) {
                throw new IllegalArgumentException("cannot add " + currency + " and " + other.currency);
            }
            return new Money(Math.addExact(units, other.units), currency);
        }

        // Split by ratios. The sum of the parts is always the whole amount.
        List<Money> allocate(long... ratios) {
            long total = 0;
            for (long r : ratios) total += r;
            long[] parts = new long[ratios.length];
            long left = units;
            for (int i = 0; i < ratios.length; i++) {
                parts[i] = units * ratios[i] / total;
                left -= parts[i];
            }
            for (int i = 0; left > 0; i++, left--) parts[i] += 1; // the units that are left, one each
            List<Money> out = new ArrayList<>();
            for (long p : parts) out.add(new Money(p, currency));
            return out;
        }

        @Override
        public String toString() {
            int d = currency.getDefaultFractionDigits(); // 2 for EUR, 0 for JPY
            if (d == 0) return units + " " + currency;
            long scale = (long) Math.pow(10, d);
            return String.format("%d.%0" + d + "d %s", units / scale, units % scale, currency);
        }
    }

    static String show(List<Money> parts) {
        return parts.stream().map(Money::toString).collect(Collectors.joining(", "));
    }

    public static void main(String[] args) {
        Money a = Money.of(1000, "EUR");
        Money b = Money.of(50, "EUR");
        System.out.println("a: " + a);
        System.out.println("a + b: " + a.plus(b));
        try {
            a.plus(Money.of(500, "JPY"));
        } catch (IllegalArgumentException e) {
            System.out.println("error: " + e.getMessage());
        }
        System.out.println("fee: " + Money.of(500, "JPY"));
        System.out.println("5 cents 70/30: " + show(Money.of(5, "EUR").allocate(70, 30)));
        System.out.println("100 EUR in 3: " + show(Money.of(10000, "EUR").allocate(1, 1, 1)));
        System.out.println("equal: " + (Money.of(1050, "EUR").equals(a.plus(b)) ? "yes" : "no"));
    }
}
```

```output
a: 10.00 EUR
a + b: 10.50 EUR
error: cannot add EUR and JPY
fee: 500 JPY
5 cents 70/30: 0.04 EUR, 0.01 EUR
100 EUR in 3: 33.34 EUR, 33.33 EUR, 33.33 EUR
equal: yes
```

- Verdict: stdlib. `java.util.Currency` gives the ISO 4217 code and the default number of fraction digits, and the record adds the amount. `Math.addExact` throws an exception on overflow.
- For amounts with more digits than a `long` holds, or for rates, use `BigDecimal`.

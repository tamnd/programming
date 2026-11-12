```java run id=split-the-steps
import java.util.List;
import java.util.Locale;

public class Main {
    record Line(long price, int qty) {}
    record Order(String customer, List<Line> lines) {}

    // Before: one method, with comments for the steps.
    static String invoiceBefore(Order order) {
        // add up the lines
        long subtotal = 0;
        for (Line line : order.lines()) subtotal += line.price() * line.qty();
        // discount for large orders
        long discount = 0;
        if (subtotal >= 10000) discount = Math.round(subtotal / 10.0);
        // tax on the discounted amount
        long tax = Math.round((subtotal - discount) / 5.0);
        // format
        long total = subtotal - discount + tax;
        return order.customer() + ": " + String.format(Locale.ROOT, "%.2f", total / 100.0);
    }

    // After: each comment became the name of a method.
    private static long subtotal(List<Line> lines) {
        return lines.stream().mapToLong(l -> l.price() * l.qty()).sum();
    }

    private static long discount(long cents) {
        return cents >= 10000 ? Math.round(cents / 10.0) : 0;
    }

    private static long tax(long cents) { return Math.round(cents / 5.0); }

    private static String formatCents(long cents) {
        return String.format(Locale.ROOT, "%.2f", cents / 100.0);
    }

    static String invoice(Order order) {
        long gross = subtotal(order.lines());
        long net = gross - discount(gross);
        return order.customer() + ": " + formatCents(net + tax(net));
    }

    interface Invoice { String of(Order order); }

    static void check(String name, Invoice f) {
        Order large = new Order("Ada", List.of(new Line(4000, 2), new Line(2500, 1)));
        Order small = new Order("Bob", List.of(new Line(1500, 2)));
        if (!f.of(large).equals("Ada: 113.40") || !f.of(small).equals("Bob: 36.00") || discount(9999) != 0) {
            throw new AssertionError(name);
        }
        System.out.println(name + ": tests pass");
    }

    public static void main(String[] args) {
        check("invoiceBefore", Main::invoiceBefore);
        check("invoice", Main::invoice);
    }
}
```

```output
invoiceBefore: tests pass
invoice: tests pass
```

- `String.format` uses the default locale, which can print a comma as the decimal mark. `Locale.ROOT` fixes the format, and in the "after" version the fix is in one method.
- `subtotal / 10` with two `long` values would truncate, so the code divides by `10.0` and rounds with `Math.round`.

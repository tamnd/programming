```java run id=gather-the-codes
import java.util.Optional;
import java.util.Set;

public class Main {
    // Before: three methods each know the codes.
    static long priceBefore(long cents, String code) {
        if (code.equals("SPRING10")) return Math.round(cents * 0.9);
        if (code.equals("VIP25")) return Math.round(cents * 0.75);
        return cents;
    }

    static String receiptBefore(String code) {
        if (code.equals("SPRING10")) return "SPRING10 (10% off)";
        if (code.equals("VIP25")) return "VIP25 (25% off)";
        return "no discount";
    }

    static boolean isValidBefore(String code) {
        return Set.of("SPRING10", "VIP25", "SUMMER20").contains(code);
    }
    // SUMMER20 was added to the validation, but not to the price or the receipt.

    // After: one enum owns the codes and their percentages.
    enum Discount {
        SPRING10(10), VIP25(25), SUMMER20(20);

        final int percent;

        Discount(int percent) { this.percent = percent; }

        static Optional<Discount> of(String code) {
            for (Discount d : values()) if (d.name().equals(code)) return Optional.of(d);
            return Optional.empty();
        }
    }

    static boolean isValid(String code) { return Discount.of(code).isPresent(); }

    static long price(long cents, String code) {
        int off = Discount.of(code).map(d -> d.percent).orElse(0);
        return Math.round(cents * (100 - off) / 100.0);
    }

    static String receipt(String code) {
        return Discount.of(code).map(d -> d.name() + " (" + d.percent + "% off)").orElse("no discount");
    }

    static void check(boolean ok, String what) {
        if (!ok) throw new AssertionError(what);
    }

    public static void main(String[] args) {
        check(isValidBefore("SUMMER20") && priceBefore(1000, "SUMMER20") == 1000, "before");
        System.out.println("before: one of three edits was made");

        check(price(1000, "SPRING10") == 900 && price(1000, "SUMMER20") == 800, "price");
        check(receipt("SUMMER20").equals("SUMMER20 (20% off)"), "receipt");
        check(price(1000, "BOGUS") == 1000 && receipt("BOGUS").equals("no discount"), "unknown");
        System.out.println("after: tests pass");
    }
}
```

```output
before: one of three edits was made
after: tests pass
```

- An `enum` makes the list of codes a type. A `switch` on `Discount` in another class can be checked by the compiler for missing cases, which a list of strings cannot.
- `Discount.of` loops over `values()` in place of `Discount.valueOf`, because `valueOf` throws `IllegalArgumentException` for an unknown name.
- The percentage is a field of each constant, so the receipt and the price read the same number.

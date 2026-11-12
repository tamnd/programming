```java run id=encapsulate-global
public class Main {
    // Before: any code can write the public static field.
    static class Globals {
        public static double taxRate = 0.2;
    }

    static long priceBefore(long cents) { return Math.round(cents * (1 + Globals.taxRate)); }

    static long exportOrderBefore(long cents) {
        Globals.taxRate = 0; // the export is tax free
        return priceBefore(cents); // and nothing puts the rate back
    }

    // After: the field is private, and the setter checks the value.
    static final class TaxSettings {
        private static double rate = 0.2;

        private TaxSettings() {}

        static double rate() { return rate; }

        static void setRate(double r) {
            if (!(r >= 0 && r <= 1)) throw new IllegalArgumentException("tax rate " + r);
            rate = r;
        }
    }

    static long price(long cents, double rate) { return Math.round(cents * (1 + rate)); }
    static long price(long cents) { return price(cents, TaxSettings.rate()); }
    static long exportOrder(long cents) { return price(cents, 0); }

    static void check(boolean ok, String what) {
        if (!ok) throw new AssertionError(what);
    }

    public static void main(String[] args) {
        check(priceBefore(1000) == 1200, "before");
        check(exportOrderBefore(1000) == 1000, "export");
        check(priceBefore(1000) == 1000, "the domestic price has changed");
        System.out.println("before: the export changed the domestic price");

        check(price(1000) == 1200 && exportOrder(1000) == 1000 && price(1000) == 1200, "after");
        try {
            TaxSettings.setRate(20);
            throw new AssertionError("rate of 20 accepted");
        } catch (IllegalArgumentException expected) {
            System.out.println("after: tests pass");
        }
    }
}
```

```output
before: the export changed the domestic price
after: tests pass
```

- Java has no global variables, but a `public static` field that is not `final` has the same problem: any class can write it, and the field has one value for the whole program.
- Java has no default parameter values, so the "after" version uses two overloads of `price`. The overload without a rate is the only code that reads the setting.
- In the setter, `!(r >= 0 && r <= 1)` also rejects `NaN`, because each comparison with `NaN` is false. `r < 0 || r > 1` would let `NaN` through.
